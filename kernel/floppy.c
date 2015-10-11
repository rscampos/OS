#include "common.h"
#include "isr.h"
#include "floppy.h"

static u32int _fdctrl_irq;
u32int _currentdrive;

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

void init_fdctrl(){
        _fdctrl_irq = 0;
        _currentdrive = 0;

        register_interrupt_handler(IRQ6,&fdctrl_callback);
        fdctrl_init_dma();
        fdctrl_reset();
        fdctrl_drive_data(13, 1, 0xf, 1);
}
