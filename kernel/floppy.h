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

const int DMA_BUFFER            =       0x6000;
