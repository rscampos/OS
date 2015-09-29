/* Names were get from qemu FDC (hw/block/fdc.h)
*/

enum{
        FDRIVE_RATE_500K = 0x00, /* 500 Kbps */
        FDRIVE_RATE_300K = 0x01, /* 300 Kbps */
        FDRIVE_RATE_250K = 0x02, /* 250 Kbps */
        FDRIVE_RATE_1M   = 0x03, /*   1 Mbps */
};

enum{
        FD_REG_DOR      = 0x3f2, /* Digital Output Register */
        FD_REG_MSR      = 0x3f4, /* Main Status Register */
        FD_REG_FIFO     = 0x3f5, /* data register */
        FD_REG_CCR      = 0x3f7, /* AT only. Configuation Control Register */
};

enum{
        FD_DOR_SELMASK  = 0x03, /* SELection driver MASK- 00000011 */
        FD_DOR_nRESET   = 0x04, /* not RESET - enable   - 00000100 */
        FD_DOR_DMAEN    = 0x08, /* DMA mode ENable      - 00001000 */
        FD_DOR_MOTEN0   = 0x10, /* MOTor ENable 0       - 00010000 */
        FD_DOR_MOTEN1   = 0x20, /* MOTor ENable 1       - 00100000 */
        FD_DOR_MOTEN2   = 0x40, /* MOTor ENable 2       - 01000000 */
        FD_DOR_MOTEN3   = 0x80, /* MOTor ENable 3       - 10000000 */
};

enum{
        FD_MSR_DRV0BUSY = 0x01, /* FDD 0 is busy on seek  - 00000001 */
        FD_MSR_DRV1BUSY = 0x02, /* FDD 1 is busy on seek  - 00000010 */
        FD_MSR_DRV2BUSY = 0x04, /* FDD 2 is busy on seek  - 00000100 */
        FD_MSR_DRV3BUSY = 0x08, /* FDD 3 is busy on seek  - 00001000 */
        FD_MSR_CMDBUSY  = 0x10, /* FDC busy on read/write - 00010000 */
        FD_MSR_NONDMA   = 0x20, /* FDC not in DMA mode    - 00100000 */
        FD_MSR_DIO      = 0x40, /* FDC has data for CPU   - 01000000 */
        FD_MSR_RQM      = 0x80, /* Data register is ready for data transfer - 00000001 */
};

enum{      
        FD_CMD_READ_TRACK               = 0x02,
        FD_CMD_SPECIFY                  = 0x03,
        FD_CMD_SENSE_DRIVE_STATUS       = 0x04,
        FD_CMD_WRITE                    = 0x05,
        FD_CMD_READ                     = 0x06,
        FD_CMD_RECALIBRATE              = 0x07,
        FD_CMD_SENSE_INTERRUPT_STATUS   = 0x08,
        FD_CMD_WRITE_DELETED            = 0x09,
        FD_CMD_READ_ID                  = 0x0a,
        FD_CMD_READ_DELETED             = 0x0c,
        FD_CMD_FORMAT_TRACK             = 0x0d,
        FD_CMD_SEEK                     = 0x0f,
};


enum{
        FDC_CMD_EXT_SKIP        =       0x20,   /* Skip deleted data address marks - 00100000 */
        FDC_CMD_EXT_DENSITY     =       0x40,   /* Operate in MFM (Double Density) mode - 01000000 */
        /* Multitrack mode - The controller will switch automatically from
         * Head 0 to Head 1 at the end of the track.
         */
        FDC_CMD_EXT_MULTITRACK  =       0x80,   };

enum FLPYDSK_GAP3_LENGTH {
        FLPYDSK_GAP3_LENGTH_STD =       0x2A,
        FLPYDSK_GAP3_LENGTH_5_14=       0x20,
        FLPYDSK_GAP3_LENGTH_3_5 =       0x1B
};

enum FLPYDSK_SECTOR_DTL {
        FLPYDSK_SECTOR_DTL_128  =       0x0,
        FLPYDSK_SECTOR_DTL_256  =       0x1,
        FLPYDSK_SECTOR_DTL_512  =       0x2,    /* most common */
        FLPYDSK_SECTOR_DTL_1024 =       0x4
};

enum FLPYDSK_INFOS{
        FLPY_SECTORS_PER_TRACK  =       0x12

};

const int DMA_BUFFER            =       0x1000;


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
