#include "common.h"
#include "paging.h"
#include "isr.h"
#include "kheap.h"

u32int number_frames    = 0;    /* number of frames available. */
int paging_enable       = 0;
extern page_directory_t *kernel_directory;

/*
 * Param 1) This is the linear address.
 * Param 2) Create a PT if there isn't. If the requested page were in a PT
 * that was already created, so, there isn't no need to create a new PT.
 * Param 3) Point to the PD.
 */
page_t * get_page(u32int address,int make,page_directory_t * page_dir){
        u32int idx_frame;
        u32int index_page = address / 0x1000;
        page_table_t * temp_table;
        if(page_dir->tables[index_page/1024]){  /* check if there is the PT */
                /* if there is the PT, the position for the page will be returned */
        }else if(make){ /* create a new PT and assigned to the PD */
                u32int temp;
                page_table_t * temp_table;
                page_t * temp_page;
                temp = kmalloc_a(sizeof(page_table_t),1);
                //printf("PT#%d created at:0x%x\n",index_page/1024, temp);
                if(paging_enable==1){ /* temp is a page - need to get the phy addr.*/
                        temp_page = get_page(temp,PT_CREATE,kernel_directory);
                        page_dir->tables[index_page/1024]               = (page_table_t*)temp;
                        page_dir->tablesPhysical[index_page/1024]       = (page_table_t*)((temp_page->frame_address << 12) | 0x5);

                }else{ /* temp is just a physical address*/
                        page_dir->tables[index_page/1024]               = (page_table_t*)temp;
                        page_dir->tablesPhysical[index_page/1024]       = temp | 0x5;
                }
                memset(temp,0,sizeof(page_table_t)); /* this line is going to trigger the #PF */
        }else{
                return 0;
        }

        return &page_dir->tables[index_page/1024]->pages[index_page%1024];
}

/* Param 1) The page.
 * Param 2) The virtual address of the page.
 */
void get_page_flgs(page_t * page){
        /* if the page isn't present, all other fields
         * are ignored. */
        if (page->present){
                puts("present ");
        }else{
                puts("not present ");
                return;
        }
        if (page->rw) puts("read-only ");
        if (page->us) puts("user-mode ");
        if (page->accessed) puts("acessed ");
        if (page->dirty) puts("dirty ");
}

void show_page(page_t * page, u32int page_va, page_directory_t * page_dir){

        int pde_idx     = (page_va/4096)/1024;
        u32int pde_addr = (u32int)page_dir + pde_idx*4;
        u32int pt_addr  = page_dir->tables[pde_idx];
        int pte_idx     = (page_va/4096)%1024;
        u32int pte_addr = pt_addr + pte_idx*4;

        printf("[+] Page info: (0x%x)\n", page_va);
        printf(" [+] Page Dir (PD)  : 0x%x\n", page_dir);
        printf("  [-] PD Entry (PDE): idx:%d Addr:0x%x\n", pde_idx, pde_addr);
        printf(" [+] Page Table (PT#%d): Addr:0x%x\n", pde_idx,pt_addr);
        printf("  [-] PT Entry (PTE): idx:%d Addr:0x%x\n", pte_idx, pte_addr);
        printf(" [+] Page struct:\n");

        /* Show the page flags*/
        printf("  [-] Flags: ");
        get_page_flgs(page);
        printf("\n");

        printf("  [-] Frame Address:0x%x\n",page->frame_address * 4096);
}

void enable_paging(){

        asm volatile(   "movl %cr0, %eax;"
                        "orl $0x80000000,%eax;"
                        "movl %eax, %cr0;"
        );

        kmalloc_init();   /* heap initialization */
}

void load_page_dir(page_directory_t *dir){

        asm("movl %0, %%cr3;"::"r" (&dir->tablesPhysical));

        enable_paging();
}

/* Show the error caused in page_fault */
void err_page_fault(u32int error){
        int present     = error & 0x1;  /* 0 - not present; 1 - present */
        int rw          = error & 0x2;  /* 0 - was read; 1 - was write */
        int us          = error & 0x4;  /* 0 - was in ring 0; 1 - was in ring 3 */
        int reserved    = error & 0x8;  /* 0 - not overwritten CPU-reserved bits; 1 - overwritten */
        int fetch       = error & 0x10; /* 0 - not caused by an instruction fetch; 1 - caused */
        if (present){
                puts("present ");
        }else{
                puts("'not present' ");
        }
        if (rw)
                puts("'on write' ");
        else
                puts("'on read' ");

        if (us)
                puts("'from user-mode' ");
        else
                puts("'from kernel-mode' ");

        if (reserved)
                puts("Overwrite reserved ");

        if (fetch)
                puts("Instruction fetch ");

}

/* Exception handler for page fault */
void page_fault(registers_t regs){
        page_t * temp_page;
        u32int fault_linear_address;

        asm volatile("movl %%cr2, %0;":"=r" (fault_linear_address));
        /*
        printf("[KERNEL] Reason: ");
        err_page_fault(regs.err_code);
        puts("\n");
        printf("[KERNEL] #PF at opcode addr.: 0x%x - access on:0x%x\n", regs.eip, fault_linear_address);
        */

        asm(   "xor %eax, %eax;"
                        "movl %eax, %cr2;"
                        "movl %cr2, %eax;"
                        );
        temp_page = get_page(fault_linear_address,PT_CREATE,kernel_directory);
        pmm_alloc_frame(temp_page, PAGE_USER, PAGE_WRITE);
}
