#include "common.h"
#include "vfs.h"
#include "fat12.h"
#include "isr.h"
#include "floppy.h"

fs_bootloader_t *fat12;

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

void show_tree(fs_root_dir_t *fat12_rd){
        u16int next_cluster;

        printf("/\n");
        while(1){
                if(fat12_rd->file_name[0] == 0x00) break; /* is the last? */

                printf("|-- %c%c%c%c%c%c%c%c",fat12_rd->file_name[0],fat12_rd->file_name[1],
                                fat12_rd->file_name[2],fat12_rd->file_name[3],fat12_rd->file_name[4],
                                fat12_rd->file_name[5],fat12_rd->file_name[6],fat12_rd->file_name[7]);

                printf(".%c%c%c",fat12_rd->file_ext[0],fat12_rd->file_ext[1],fat12_rd->file_ext[2]);

                printf(" (%s size:%d cluster:%d)",(fat12_rd->file_attrib == 0x20 ? "FILE" : "DIR "),
                                fat12_rd->file_size, fat12_rd->file_first_cluster);

                printf(" (Next cluster:");
                next_cluster = get_next_cluster(fat12_rd->file_first_cluster);
                while(next_cluster  > 0){
                        printf(" %d",next_cluster);
                        next_cluster = get_next_cluster(next_cluster);
                }
                printf(")\n");

                if((fat12_rd->file_attrib == 0x10) && fat12_rd->file_name[0] != '.')
                        show_tree(get_fat12_rootdir(fat12_rd->file_first_cluster));

                //if(fat12_rd->file_size <= 600)
                //        show_content(fat12_rd);        

                fat12_rd++;
                //puts("\n");
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

int read_fs_fat12(vfs_node_t *node, void *buffer, u32int size){
        printf("Read callback called.\n");
}

int write_fs_fat12(vfs_node_t *node, void *buffer, u32int size){
        printf("Read callback called.\n");
}

int close_fs_fat12(vfs_node_t *node){
        printf("Close callback called.\n");
}

void init_fat12(){

        fs_root_dir_t   *fat12_rd;
        void            *floppy_fat12, *temp;
        int             sectors=2880, i=0;

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

        /* show files */
        show_tree(fat12_rd);

        //printf("[+] floppy end!\n");
}
