/* hal.c -- First function to be called when enters in kernel program
 * The function setuphal() set up the kernel.
 */

//#include "multiboot.h"
#include "pmm.h"

int setuphal(multiboot_info_t* bootinfo)
{
        /* Screen "inicialization" */
        clear_screen();

        /* Create the borders*/
        vga_banner_top("                           --==[ Operation System ]==--                          ");
        vga_banner_bottom("                       --==[ Executing all process... ]==--                      ");

        /* GDT e IDT inicialization */
        puts("[HAL] Set up GDT, IDT and PIC remmap...");
	init_descriptor_tables();
        puts("done.\n");

        /* IRQs inicialization */
        puts("[HAL] Installing IRQs...");
        init_keyboard();
        init_timer(20000000);
        puts("done.\n");
        
	/* Execptions inicialization */
        puts("[HAL] Installing Execptions...");
        init_exceptions();
        puts("done.\n");

        /* Turn on the interrupts - Let's start the kernel */
        puts("[HAL] Enabling interrupt...");
        asm volatile("sti");
        puts("done.\n");

        /* Mapping virtual address to frames */
        puts("[HAL] Mapping the memory...");
        pmm_memory_map(bootinfo);
        puts("done.\n");


        /* Setup the floppy driver */
        puts("[HAL] Floppy controller initialization...");
        init_fdctrl();
        puts("done.\n");

        /* Init fat12 */
        puts("[HAL] Loading file system (FAT12)...");
        init_fat12();
        puts("done.\n");
	

	/* Init all the process */
        puts("[HAL] Init process\n");
	init_process();

	/* Syscall init */
	init_syscall();
	
	/* PCI Routines */
        //init_pci();

	return 1;
}
