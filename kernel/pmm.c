#include "paging.h"
#include "pmm.h"
#include "common.h"
#include "isr.h"

#define PMM_BITMAP_SCHUNCK      8               /* Size of the chunck (in bits) */
#define PMM_MEM_TO_M_B(x)       (x*1024*1024)
#define PMM_MEM_TO_B_M(x)       (x/(1024*1024))
#define PMM_MEM_TO_B_K(x)       (x/(1024))

extern u32int phy_addr;                 /* defined in kheap.c                           */
extern u32int number_frames;            /* defined in paging.c                          */
u32int size_bitmap;                     /* size of bytemap (in bytes)                   */
u32int memory_len;                      /* Total of phy memory (in bytes)               */
u32int frames_used;                     /* number of used frames                        */
u32int pages_used;                      /* number of used pages                         */
u32int *bitmap;                         /* Pointer to bitmap address (phy addr.)        */
page_directory_t *kernel_directory;     /* Page directory of the kernel                 */

char * pmm_type(u32int mmtype){
        switch(mmtype){
                case AVAILABLE:
                        return "Available";
                        break;

                case RESERVED:
                        return "Reserved";
                        break;
                case ACPI_RECLAIM:
                        return "ACPI Reclaim";
                        break;
                case ACPI_NVS_MEMORY:
                        return "ACPI NVS Memory";
                        break;
                default:
                        return "Undefined";
        }
}

/* The first function to be called. This function sets the bitmap
 * and call the init_page.
 */
void pmm_memory_map(multiboot_info_t* bootinfo){
        int region;

        u32int available_memory=0, used_memory=0;
        memory_len = bootinfo->m_memoryLo + 1;

        /* Pointer to the memory map */
        memory_map_t * p_memory_map = bootinfo->m_mmap_addr;
        memory_len = ((memory_len*64)/1024)+16;

        /* Inicalize the bitmap for the physical memory */
        pmm_init_bitmap();
        pmm_init_page();

}

void pmm_init_page(){
        u32int virt_addr = 0x0;
        u32int address;
        page_t * temp;

        /* Alloc and clean memory for PD (Page Directory) */
        kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t), 1);
        memset(kernel_directory, 0, sizeof(page_directory_t));

        /* All the PTs are reserved after the PD */
        while(virt_addr < phy_addr){
                pmm_alloc_frame(get_page(virt_addr, 1, kernel_directory), PAGE_USER, PAGE_WRITE);
                virt_addr += PAGE_SIZE;
        }

        register_interrupt_handler(ISR14, &page_fault);
        load_page_dir(kernel_directory);

        /* just a test */
        virt_addr = 0x0804A000;
        temp = get_page(virt_addr, 1, kernel_directory);
        pmm_alloc_frame(temp, PAGE_USER, PAGE_WRITE);
        memset(virt_addr, 0x90, 20);
        printf("Address:&0x%x[%x]\n", &temp, temp);
        printf("Frame addr.:[%x]\n", temp->frame_address);
}

void pmm_init_bitmap(){
        u32int physical_addr=0x0;
        int i;

        /* Alloc memory for bitmap */
        number_frames = PMM_MEM_TO_M_B(memory_len)/PMM_FRAME_SIZE;
        bitmap = kmalloc(number_frames/PMM_BITMAP_SCHUNCK);
        memset(bitmap,0,number_frames/PMM_BITMAP_SCHUNCK);

        /* From 0 to 1MB, there's just 159 (0 to 158)frames
         * avaiable. It's start at 0x0 and go untill 0x9E000,
         * actually the range 0x0~0x9FC00 is avaiable, but the last
         * avaible page is at address 0x9E000~0x9F000. After
         * that, all frames are reserved untill the address
         * 0x100000. At 0x100000 untill the top of the physical
         * memory, all the frames are avaiable.
         */

        /* 0x9F000(159*4KB) ~  0x100000(256*4KB)*/
        //for(i=159;i<256;i++)
          //      pmm_set_bitmap(i);

}

u32int pmm_used_memory(){
        int i=0;
        int bits_alloc=0;
        u32int used_memory=0x0;

        for(i;i<number_frames;i++){
                if(check_frame(i))
                        bits_alloc++;
                        used_memory+=PMM_FRAME_SIZE;
        }
        printf("Bitmap count:%d\n",bits_alloc);
        return used_memory;
}

/* pmm_alloc_frame recives a 'page' (PTE location) and get a free frame calling
 * pmm_first_free() which returns a physical address (of a free frame).
 * This physical address is put in page->frame_address.
 */
void pmm_alloc_frame(page_t * page, int is_kernel, int is_writeable){
        u32int idx_frame, address_frame;

        if(page->frame_address != 0)
                return; /* frame already alloced */

        pages_used++; /* number of pages alloced */

        idx_frame = pmm_first_free();
        pmm_set_bitmap(idx_frame);
        address_frame = idx_frame*PAGE_SIZE;
        //printf("Page alloced:0x%x Frame:0x%x\n", page, address_frame);

        /* Set the page */
        page->frame_address     = address_frame >> 12; /* same as idx_frame */
        page->present           = 1;
        page->us                = (is_kernel)?0:1;
        page->rw                = (is_writeable)?1:0;
}

void pmm_free_frame(page_t * page){
        u32int idx_frame;
        idx_frame = page->frame_address;
        page->frame_address = 0x0;
        pmm_clear_bitmap(idx_frame);
}


void pmm_set_bitmap(int index){
        char byte_bitmap, flag=0x1;
        int index_bitmap;
        index_bitmap = index/8;
        byte_bitmap = bitmap[index_bitmap];
        flag = flag << (index-(index_bitmap*8));
        bitmap[index_bitmap] = bitmap[index_bitmap] + flag;
        frames_used++;
}

void pmm_clear_bitmap(int index){
        char byte_bitmap, flag=0x1;
        int index_bitmap;
        index_bitmap = index/8;
        byte_bitmap = bitmap[index_bitmap];
        flag = flag << (index-(index_bitmap*8));
        bitmap[index_bitmap] = bitmap[index_bitmap] - flag;
}

int check_frame(int index){
        char byte_bitmap, flag=0x1;
        int index_bitmap;
        index_bitmap = index/8;
        byte_bitmap = bitmap[index_bitmap];
        flag = flag << (index-(index_bitmap*8));
        if(bitmap[index_bitmap] & flag)
                return 1;       /* alloced */
        else
                return 0;       /* free */
}

int pmm_first_free(){
        int i=0,j=0,found=0;
        char byte_bitmap;
        while(!found){
                byte_bitmap = bitmap[i] ^ 0xFF; /* now bit 1 is free */
                for(j=0;j<8;j++){

                        if(byte_bitmap & 0x1){
                                found = 1;
                                break;
                        }
                        byte_bitmap = byte_bitmap >> 1;
                }
                i++;
        }
        return 8*(i-1)+j;
}
