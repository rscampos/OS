int read_fs_fat12(vfs_node_t *node, void *buffer, u32int size);
int write_fs_fat12(vfs_node_t *node, void *buffer, u32int size);
int close_fs_fat12(vfs_node_t *node);

/* Root Directory Struct */
typedef struct fs_root_dir
{
        unsigned char   file_name[8];
        unsigned char   file_ext[3];
        unsigned char   file_attrib;
        unsigned char   file_reserved;          /* Reserved for Windows NT      */
        unsigned char   file_ctime_ms;
        unsigned short  file_ctime_smh;         /* Second, Minutes, Hours       */
        unsigned short  file_cyear_hmy;         /* Hours, Month, Year           */
        unsigned short  file_last_access;       /* Last access date             */
        unsigned short  file_ea_index;
        unsigned short  file_last_mod_time;     /* Last modified time           */
        unsigned short  file_last_mod_date;     /* Last modified date           */ 
        unsigned short  file_first_cluster;
        unsigned int    file_size;
}__attribute__((packed)) fs_root_dir_t;


typedef struct fs_bootloader
{
        unsigned char   jump[3];
        unsigned char   bpb_oem[8];             /* OEM Identifier */
        unsigned short  bpb_bytes_sector;       /* Bytes per Sector */
        unsigned char   bpb_sectors_cluster;    /* Sectors per Cluster */
        unsigned short  bpb_reserved_sec;       /* Reserved Sectors */
        unsigned char   bpb_fatnumbers;         /* Numbers of FAT */
        unsigned short  bpb_root_entries;       /* Root Entries */
        unsigned short  bpb_total_sectors;      /* 2880 for FAT12 */
        unsigned char   bpb_media;              /* byte code information */
        unsigned short  bpb_sectors_fat;        /* Sectors per FAT - 9 */
        unsigned short  bpb_sectors_track;      /* Sectors per Track - 18 */
        unsigned short  bpb_head_track;         /* Heads per Track - 2 */
        unsigned int    bpb_hidden_sectors;     /* Usually 0 */
        unsigned int    bpb_total_sec_big;      /* Total Sectors Big */

        /* ext */
        unsigned char   bpb_ext_driver_number;  /* Driver Number */
        unsigned char   bpb_ext_unused;         /* Unused - 0 */
        unsigned char   bpb_ext_signature;      /* Ext Signature - 0x28||0x29 */
        unsigned int    bpb_ext_serial_number;  /* Serial number - 0x41384022 */
        unsigned char   bpb_ext_vol_label[11];  /* Volume Label */
        unsigned char   bpb_ext_file_system[8]; /* File System Name */
        unsigned char   boot_code[448];         /* Boot Code */
        unsigned short  bootable_partition;     /* 0xAA55 */
}__attribute__((packed)) fs_bootloader_t;

unsigned short get_next_cluster(u16int cluster);
fs_root_dir_t * get_fat12_rootdir(unsigned short each_rd);
void show_tree(fs_root_dir_t *fat12_rd);
void show_bootsector(fs_bootloader_t *fat12);
