#include "common.h"
#include "paging.h"
#include "pmm.h"

extern u32int end;
extern int paging_enable;
extern page_directory_t *kernel_directory;
extern u32int frames_used;
extern u32int number_frames;
u32int phy_addr = (u32int)&end;
u32int kheap_init_addr = 0x30000000; /* PT#192 */
/* Param 1) size to be alloced.
 * Param 2) align the size of the page. Align first and than alloc.
*/
u32int kmalloc_a(u32int size, int align){
        u32int temp;
        page_t * page_temp;

        /* If the paging isn't enable, the phy_addr starts after
         * the last byte in the KERNEL IMG.
         */
        if(paging_enable!=1){
                if(align == 1 && ((phy_addr & 0xFFFFF000)!= phy_addr)){ /* checks if isn't already page-aligned */
                        phy_addr &= 0xFFFFF000;
                        phy_addr += PAGE_ALIGN;
                }       
                
                temp      = phy_addr;
                phy_addr += size;
        }else{

                if(align == 1 && ((kheap_init_addr & 0xFFFFF000)!= kheap_init_addr)){ /* checks if isn't already page-aligned */
                        kheap_init_addr &= 0xFFFFF000;
                        kheap_init_addr += PAGE_ALIGN;
                }       
                temp            = kheap_init_addr;
                kheap_init_addr += size;

                /* Show the base address and the size alloced.
                 * The number of used frames and availables.
                 */
                page_temp = get_page(temp,1,kernel_directory);
                pmm_alloc_frame(page_temp, PAGE_KERNEL, PAGE_READ);
                
                /* 
                printf("[+] kmalloc called:\n");
                printf("  [-] base_addr : 0x%x\n", temp);
                printf("  [-] size      : %d\n", size);
                printf("  [-] final_addr: 0x%x\n",kheap_init_addr);
                printf("  [-] aval:%d used:%d\n",number_frames,frames_used);
                */
             }
        
        return temp;
}

u32int kmalloc(u32int size){
        /* Call kmalloc_a() without align. */
        return kmalloc_a(size,0);
}

void kmalloc_init(){
        page_t * page;
        pmm_alloc_frame(get_page(kheap_init_addr,1,kernel_directory), PAGE_KERNEL, PAGE_READ);
        page = get_page(kheap_init_addr,1,kernel_directory);
        paging_enable = 1;
}
