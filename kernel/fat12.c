#include "common.h"
#include "vfs.h"
#include "fat12.h"
#include "isr.h"
#include "floppy.h"

fs_bootloader_t *fat12;
vfs_node_t *root_fs;
vfs_node_t *subdir_temp;
u8int root_fs_lock;
u16int index_inode;

/* Nodes callbacks */
int read_fs_fat12(vfs_node_t *node, void *buffer, u32int size){
        unsigned int offset, content, source;

        offset  = (node->first_cluster - 2);
        content = (0x4200/0x200) + offset;
        source  = fat12 + content;

        printf("Read callback called: %s 0x%x %d\n", node->filename, buffer, size);
        memcpy(buffer,source,size);
        return 0;
}

int write_fs_fat12(vfs_node_t *node, void *buffer, u32int size){
        printf("Write callback called: %s 0x%x %d\n", node->filename, buffer, size);
        return 0;
}

int close_fs_fat12(vfs_node_t *node){
        printf("Close callback called: %s\n", node->filename);
        return 0;
}

/* Return the next cluster */
unsigned short get_next_cluster(u16int cluster){
        u16int next_cluster, offset;
        void * addr_fat;

        offset   = cluster * 1.5;
        addr_fat = fat12 + fat12->bpb_reserved_sec;
        addr_fat += offset;

        memcpy(&next_cluster, addr_fat, sizeof(u16int));

        if(!(cluster % 2))
                next_cluster = next_cluster & 0x0FFF;
        else
                next_cluster = (next_cluster & 0xFFF0) >> 4;

        return next_cluster;
}

/* if 0 retunrs the rootdir; else (subdir case)*/
fs_root_dir_t * get_fat12_rootdir(unsigned short each_rd){
        unsigned int offset_root_dir;

        offset_root_dir = fat12->bpb_reserved_sec + (fat12->bpb_fatnumbers * fat12->bpb_sectors_fat);

        if(each_rd != 0) /* subdir */
                offset_root_dir += ((each_rd - 2) + 0xE);

        return (fat12 + offset_root_dir);
}

void show_content(fs_root_dir_t *file){
        unsigned int offset, content, temp;

        offset  = (file->file_first_cluster - 2);
        content = (0x4200/0x200) + offset;
        temp    = fat12 + content;
        printf("   =>Content:0x%s\n", temp);
}

void fat12_filename_dos(fs_root_dir_t *fat12_rd, vfs_node_t *node){
        u8int index=8;
        int i;

        /* TODO - ponto apenas se tiver ext e se tiver, colocar logo apos nome */
        for(i=7;i>=0;i--){
                if(fat12_rd->file_name[i] == 0x20)
                        index--;
                else
                        break;
        }
                      
        memcpy(node->filename,fat12_rd->file_name,index);
        
        if(fat12_rd->file_ext[0] != 0x20){
                memset(&node->filename[index],'.',1);
                memcpy(&node->filename[index+1],fat12_rd->file_ext,3);
                memset(&node->filename[index+4],0x0,13-(index+4));
        }else{
                memset(&node->filename[index+4],0x0,13-(index-1)); 
        }

}

void show_tree(vfs_node_t *node, int level){
        int i;

        while(node != 0){
                for(i=0;i<level;i++) puts(" ");
                
                if((node->attr == 0x10)) 
                        printf("- %s/\n",node->filename, level); 
                else
                        printf("- %s\n",node->filename, level); 

                /* Found a subdir */
                if((node->attr == 0x10) && node->filename[0] != '.')
                        show_tree(node->subdir, ++level);

                node = node->next;
        }
}


void fat12_initialize(fs_root_dir_t *fat12_rd){
        u16int next_cluster;
        vfs_node_t *node, *prev=0;  
        vfs_node_t *subdir_node;
        
        /* get the prev subdir reference */ 
        subdir_node = subdir_temp;

        while(1){
                if(fat12_rd->file_name[0] == 0x00) break; /* is the last? */
                
                /* Create a new node */
                node = kmalloc(sizeof(vfs_node_t));
       
                fat12_filename_dos(fat12_rd, node);
                node->inode             = ++index_inode;
                node->attr              = fat12_rd->file_attrib; 
                node->size              = fat12_rd->file_size;
                node->first_cluster     = fat12_rd->file_first_cluster; 
                node->current_cluster   = fat12_rd->file_first_cluster;
                node->offset            = 0;
                /* callbacks */
                node->read              = &read_fs_fat12;
                node->write             = &write_fs_fat12;
                node->close             = &close_fs_fat12;
             
                if(prev)/* is not the first one */
                        prev->next      = node;
                else{   /* when the node is the first one (root or subdir)*/
                        if(!root_fs_lock){ /* root file */
                                root_fs = node;
                                root_fs_lock = 1;
                        }

                        if(subdir_node) /* first node of subdir */
                                subdir_node->subdir = node;
               }

                prev = node; 

                /* Found a subdir */
                if((node->attr == 0x10) && fat12_rd->file_name[0] != '.'){
                        subdir_temp = node; /* mark the subdir */
                        fat12_initialize(get_fat12_rootdir(fat12_rd->file_first_cluster));
                        subdir_temp = 0; /* no subdir anymore */
                }

                fat12_rd++;
        }
}

void show_bootsector(fs_bootloader_t *fat12){

        printf("[+] Disk information:\n");
        printf("  [-] Disk Size:%dB (%dKB)\n",
                        fat12->bpb_total_sectors * fat12->bpb_bytes_sector,
                        (fat12->bpb_total_sectors * fat12->bpb_bytes_sector)/1024);

        printf("  [-] Total of sectors:%d\n", fat12->bpb_total_sectors); 
        printf("  [-] Num of FAT's:%d | Sectors per FAT:%d | Sectors per Track:%d\n",
                        fat12->bpb_fatnumbers, fat12->bpb_sectors_fat, fat12->bpb_sectors_track);

        printf("  [-] FAT12 Reserved Sectors: %d\n",fat12->bpb_reserved_sec);
        printf("  [-] FAT12 RD (Root Dir): %d Sectors\n",
                        (fat12->bpb_root_entries * 32)/fat12->bpb_bytes_sector);

        /*
           printf("  [-] File system name:\n",
           (fat12->bpb_root_entries * 32)/fat12->bpb_bytes_sector);
           */

        printf("[+] Bootable: %s\n", (fat12->bootable_partition == 0xAA55 ? "Yes" : "No" ));
}

void init_fat12(){

        fs_root_dir_t   *fat12_rd;
        void            *floppy_fat12, *temp;
        int             sectors=2880, i=0;
        root_fs         = 0;
        subdir_temp     = 0;
        root_fs_lock    = 0;
        index_inode     = 0;

        floppy_fat12 = kmalloc(sectors * 512);
        memset(floppy_fat12, 0x00, sectors * 512);

        /* Alloc space for all floppy */
        for(i=0;i<sectors;i++){
                temp = fdctrl_read_sector(i);
                memcpy(floppy_fat12+(i*512),temp,512);
        }

        fat12 = (fs_bootloader_t *) floppy_fat12;

        /* bootsector region */
        //show_bootsector(fat12);

        /* 0 - root directory */
        fat12_rd = get_fat12_rootdir(0);

        /* Load files and dir to memory */
        fat12_initialize(fat12_rd);

        /* Show the tree directory */
//        show_tree(root_fs, 0);
        //printf("[+] floppy end!\n");
}
