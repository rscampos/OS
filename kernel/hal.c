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
        vga_banner_top("                   --==[ Physical Memory Manager (PMM) ]==--                    ");        
        vga_banner_bottom("                     --==[ Build the paging schema... ]==--                     ");     

        //puts("[HAL] Set up screen...done\n"); 
        
        /* GDT e IDT inicialization */
        //puts("[HAL] Set up GDT, IDT and PIC remmap..."); 
        init_descriptor_tables(); 
        //puts("done.\n");
       
        /* IRQs inicialization */
        //puts("[HAL] Installing IRQs...\n"); 
        //init_timer(20000000); 
        asm volatile("sti");

        //init_keyboard();
        

        
        /* Execptions inicialization */
        //puts("[HAL] Installing Execptions...");
        init_exceptions();
        //asm volatile("int3");
        //puts("done.\n");
        
        /* Turn on the interrupts - Let's start the kernel */
        //puts("[HAL] Enabling interrupt..."); 

        ///puts("done.\n");
       
        pmm_memory_map(bootinfo);  
        
        /* Setup the floppy driver */
        init_fdctrl();
        
        //init_fat12();
        u32int * alloc;
        alloc = kmalloc(200);
        //printf("teste:0x%x\n",alloc);       
       
        //asm volatile("movl %cr0, %eax");
        
        /* Ring3 test */
        ring3_test();

        /* PCI Routines */
        //init_pci(); 
        return 1;
}
