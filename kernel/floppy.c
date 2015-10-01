#include "common.h"
#include "isr.h"
#include "floppy.h"

static u32int _fdctrl_irq;
u32int _currentdrive;
fs_bootloader_t *fat12;

void fdctrl_callback(registers_t regs){
        //printf("[+] Interrupt: IRQ6 raised\n");
        _fdctrl_irq = 1;
}


void fdctrl_init_dma(){
        outb(0x0a, 0x06);                       /* mask dma channel 2 */
        outb(0xd8, 0xff);                       /* reset master flip-flop */
        outb(0x04, DMA_BUFFER & 0x00FF);        /* address=0x1000 */
        outb(0x04, (DMA_BUFFER & 0xFF00) >> 8);
        outb(0xd8, 0xff);                       /* reset master flip-flop */
        outb(0x05, 0xff);                       /* count to 0x23ff (number of ...*/
        outb(0x05, 0x23);                       /* bytes in a 3.5" floppy disk track) */
        outb(0x80, 0x00);                       /* external page register = 0 */
        outb(0x0a, 0x02);                       /* unmask dma channel 2 */

} 

void fdctrl_dma_read(){
        outb(0x0a, 0x06);       /* mask dma channel 2 */
        outb(0x0b, 0x56);       /* single transfer, address increment, autoinit, read, channel 2 */
        outb(0x0a, 0x02);       /* unmask dma channel 2 */
}

void fdctrl_dma_write(){
        outb(0x0a, 0x06);       /* mask dma channel 2 */
        outb(0x0b, 0x5a);       /* single transfer, address increment, autoinit, write, channel 2 */
        outb(0x0a, 0x02);       /* unmask dma channel 2 */

}

void fdctrl_write_dor(u8int val){
        /* write the Digital Output Register (DOR) */
        outb(FD_REG_DOR, val);
}

u8int fdctrl_read_status(){
        /* read the Main Status Register (MSR) */
        return in(FD_REG_MSR);
}

void fdctrl_write_ccr(u8int val){
        /* write to Configuation Control Register (CCR)*/
        outb(FD_REG_CCR, val);
}

u8int fdctrl_send_cmd(u8int cmd){
        int i;
        /* wait until register is ready for send the command */
        for(i=0; i < 500; i++)
                if(fdctrl_read_status() & FD_MSR_RQM)
                        return outb(FD_REG_FIFO,cmd);
}

u8int fdctrl_read_data(){
        int i;
        /* wait until register is ready for data transfer */
        for(i=0; i < 500; i++)
                if(fdctrl_read_status() & FD_MSR_RQM)
                        return in(FD_REG_FIFO);
}

/* Pass controlling information to the FDC about the machanical drive connect to it */
void fdctrl_drive_data(u32int steprate, u32int loadt, u32int unloadt, u32int dma){
        u32int data = 0;
        fdctrl_send_cmd(FD_CMD_SPECIFY);

        /* CMD Specify needs 2 parameters */
        data = (((steprate & 0xf) << 4) | (unloadt & 0xff));
        // printf("Drive data:0x%x\n",data);
        fdctrl_send_cmd(data);

        data = ((loadt & 0xff) << 1) | dma;
        // printf("Drive data:0x%x\n",data);
        fdctrl_send_cmd(data);

}

void fdctrl_wait_irq(){
        while(!_fdctrl_irq);
        _fdctrl_irq = 0;
}


void fdctrl_check_int(u32int *st0, u32int *cyl){
        fdctrl_send_cmd(FD_CMD_SENSE_INTERRUPT_STATUS);
        *st0 = fdctrl_read_data();
        *cyl = fdctrl_read_data();
}

void fdctrl_control_motor(u32int flag){

        /* Turn on/off just drive 0 */
        if(flag){
                fdctrl_write_dor(FD_DOR_MOTEN0 | FD_DOR_DMAEN | FD_DOR_nRESET);
        }else{ /* turn off */ 
                fdctrl_write_dor(FD_DOR_nRESET);
        }

}

int fdctrl_calibrate(u32int drive){
        int i;
        u32int st0, cyl;

        // printf("Calibrating the drive: %d\n", drive);

        if(drive > 3)
                return -2;

        /* turn on the motor */
        fdctrl_control_motor(1);
        for (i=0; i < 10; i++) {
                fdctrl_send_cmd(FD_CMD_RECALIBRATE); 
                fdctrl_send_cmd(drive);
                fdctrl_wait_irq();
                fdctrl_check_int(&st0,&cyl);
                //printf("FDC#%d st0=0x%x cyl=%d\n",i,st0,cyl);
                /* if cylinder is 0, it's done */
                if(!cyl){

                        fdctrl_control_motor(0);
                        return 0;
                }
        }
        /* turn off the motor */
        fdctrl_control_motor(0);

        return -1;
}

void fdctrl_reset(){
        int i;
        u32int st0, cyl;
        fdctrl_disable_controller();
        fdctrl_enable_controller();
        fdctrl_wait_irq();
        // printf("[+] Floppy Driver (%d) reseted!\n",_currentdrive);

        /* Check the status for all 4 FDCs */
        for(i=0;i<4;i++){
                fdctrl_check_int(&st0,&cyl);
                //printf("FDC#%d st0=0x%x cyl=%d\n",i,st0,cyl);
        }

        /* Setting the CCR */
        fdctrl_write_ccr(FDRIVE_RATE_500K);
        fdctrl_drive_data(3,16,240,1);

        /* Calibrate the FDD */
        fdctrl_calibrate(_currentdrive);
}

void fdctrl_lba_to_chs(int sectorLBA, int *track, int *head, int *sector){
        *head   = (sectorLBA % (FLPY_SECTORS_PER_TRACK * 2)) / (FLPY_SECTORS_PER_TRACK);
        *track  = (sectorLBA / (FLPY_SECTORS_PER_TRACK * 2));
        *sector = (sectorLBA % FLPY_SECTORS_PER_TRACK) + 1;
}

int fdctrl_seek(int track, int head){
        u32int st0, cyl;
        int i;

        for (i = 0; i < 10; i++ ){
                /* send the seek command */
                fdctrl_send_cmd(FD_CMD_SEEK);
                fdctrl_send_cmd((head << 2) | _currentdrive);
                fdctrl_send_cmd(track);

                /* interrupt */
                fdctrl_wait_irq();
                fdctrl_check_int(&st0,&cyl);
                //printf("FDC# st0=0x%x cyl=%d == track=%d\n",st0,cyl,track);
                if(cyl == track)
                        return 0;
        }

        return -1;
}


void fdctrl_read_sector_imp(int track, int head, int sector ){
        unsigned char st0, st1, st2, rcy, rhe, rse, bps;
        int i;

        fdctrl_control_motor(1); 

        /* set the DMA for read transfer */
        fdctrl_dma_read();
        //printf("C:%d H:%d S:%d\n", track, head, sector);

        //! read in a sector
        /*
           printf("Comando - read:0x%x 0x%x  track:0x%x  head:0x%x  sec:0x%x  0x%x  0x%x  0x%x 0x%x \n",(FD_CMD_READ | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_DENSITY),
           (head << 2 | _currentdrive),track, head, sector, FLPYDSK_SECTOR_DTL_512,
           (((sector + 1)>= FLPY_SECTORS_PER_TRACK )? FLPY_SECTORS_PER_TRACK : sector + 1),
           FLPYDSK_GAP3_LENGTH_3_5, 0xff);
           */

        fdctrl_send_cmd(FD_CMD_READ | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_DENSITY);
        fdctrl_send_cmd(head << 2 | _currentdrive );
        fdctrl_send_cmd(track);
        fdctrl_send_cmd(head);
        fdctrl_send_cmd(sector);

        fdctrl_send_cmd(FLPYDSK_SECTOR_DTL_512);


        fdctrl_send_cmd(
                        ( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK )
                        ? FLPY_SECTORS_PER_TRACK : sector + 1 );

        fdctrl_send_cmd(FLPYDSK_GAP3_LENGTH_3_5);

        fdctrl_send_cmd(0xff);

        fdctrl_wait_irq ();

        st0 = fdctrl_read_data(); /* Status Register 0  */
        st1 = fdctrl_read_data(); /* Status Register 1  */
        st2 = fdctrl_read_data(); /* Status Register 2  */
        rcy = fdctrl_read_data(); /* Current cylinder   */
        rhe = fdctrl_read_data(); /* Current head       */
        rse = fdctrl_read_data(); /* Sector number      */
        bps = fdctrl_read_data(); /* Sector size        */

        //printf("st0:%x %x %x cyl:%x head:%x sec:%x bps:%x\n",st0, st1, st2, rcy, rhe, rse, bps); 

        fdctrl_control_motor(0); 
}

u32int* fdctrl_read_sector(u32int sectorLBA){
        int track=0, head=0, sector=1;
        fdctrl_lba_to_chs(sectorLBA, &track, &head, &sector);
        //printf("SectorLBA:%d C:%d H:%d S:%d\n",sectorLBA, track, head, sector);
        /* turn on the motor and seek */
        fdctrl_control_motor(1);
        if (fdctrl_seek(track, head) != 0)
                return 0;
        /*  read sector and turn motor off */
        fdctrl_read_sector_imp(track, head, sector);

        fdctrl_control_motor(0);

        return (u32int*) DMA_BUFFER;
}

void fdctrl_enable_controller(){
        fdctrl_write_dor(FD_DOR_nRESET | FD_DOR_DMAEN);
}

void fdctrl_disable_controller(){
        /* just for FDC 0 */
        fdctrl_write_dor(_currentdrive);
}

/* Return the next cluster */
unsigned short get_next_cluster(unsigned short cluster){
        printf("Cluster:%d\n", cluster);
        printf("Next:%d\n", cluster);
        /*
           if(fat12_rd->file_first_cluster % 2 == 0){
           byte_offset = fat12_rd->file_first_cluster*1.5;
           }else{
           byte_offset = (fat12_rd->file_first_cluster-1)*1.5+1;
           }
           printf("    [-] Byte offset:%d\n", byte_offset);
           if(first_second){
           next_cluster = (next_cluster && 0xFFF0) >>4;
           }else{
           next_cluster = next_cluster && 0x0FFF;
           }


           buffer_init     = fdctrl_read_sector(offsec_fat1);

           next_cluster    = (buffer_init[byte_offset+1]<<4) + buffer_init[byte_offset];
           printf("0x%x 0x%x\n",buffer_init[byte_offset+1]<<4, buffer_init[byte_offset]);

           printf("0x%x \n", next_cluster); 
           */
}

void show_content(fs_root_dir_t *file){
        unsigned int offset, content, temp;

        offset  = (file->file_first_cluster - 2);
        content = (0x4200/0x200) + offset;
        temp    = fat12 + content;
        printf("   =>Content:0x%s\n", temp);
}

void show_files(fs_root_dir_t *fat12_rd){
        unsigned short next_cluster;

        printf("[+] Files:\n");
        while(1){
                if(fat12_rd->file_name[0] == 0x00) break; /* is the last? */

                printf(" + Filename: %c%c%c%c%c%c%c%c",fat12_rd->file_name[0],fat12_rd->file_name[1],
                fat12_rd->file_name[2],fat12_rd->file_name[3],fat12_rd->file_name[4],
                fat12_rd->file_name[5],fat12_rd->file_name[6],fat12_rd->file_name[7]);

                printf(".%c%c%c\n",fat12_rd->file_ext[0],fat12_rd->file_ext[1],fat12_rd->file_ext[2]);
                printf(" |- Attr:0x%x size:%d cluster:%d\n",fat12_rd->file_attrib,fat12_rd->file_size,
                                fat12_rd->file_first_cluster);

                if(fat12_rd->file_size <= 100)
                        show_content(fat12_rd);        
                
                fat12_rd++;
                puts("\n");
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

        printf("[+] Bootable: %s\n", (fat12->bootable_partition == 0xAA55 ? "Yes" : "No" ));
}

fs_root_dir_t * get_fat12_rootdir(fs_bootloader_t *fat12){
        unsigned int offset_root_dir; 
        offset_root_dir = fat12->bpb_reserved_sec + (fat12->bpb_fatnumbers * fat12->bpb_sectors_fat);
        return (fat12 + offset_root_dir);
}

void init_fdctrl(){
        _fdctrl_irq = 0;
        _currentdrive = 0;


        fs_root_dir_t   *fat12_rd;
        unsigned int    *fat12_fat1;
        unsigned int    *fat12_fat2;
        unsigned int    *fat12_data;
        void    *floppy_fat12;
        void    *temp;
        char            *buffer_bpb_oem;
        int sectors     = 2880;
        int test_cluster= 0;
        int first_second= 0;
        unsigned short int next_cluster;
        unsigned int * addr_alloc;
        unsigned int offsec_fat1, offsec_fat2, byte_offset;
        int i,j,size;

        register_interrupt_handler(IRQ6,&fdctrl_callback);
        fdctrl_init_dma();
        fdctrl_reset();
        fdctrl_drive_data(13, 1, 0xf, 1);

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

        /* root directory */
        fat12_rd = get_fat12_rootdir(fat12);

        /* show files */
        show_files(fat12_rd);

        printf("[+] floppy end!\n");
}
