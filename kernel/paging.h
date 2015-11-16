#include "common.h"

typedef struct page{
        u32int  present         :1; /* if page is present - 1 */
        u32int  rw              :1; /* if page is writable - 1 */
        u32int  us              :1; /* if page is for supervisor - 0 */
        u32int  pwt             :1;
        u32int  pcd             :1;
        u32int  accessed        :1;
        u32int  dirty           :1;
        u32int  pat             :1;
        u32int  global          :1;
        u32int  ignored         :3;
        u32int  frame_address   :20;
}page_t;

/* PT - Page Table; 1024 PTE - Entrys */
typedef struct page_table{
        page_t pages[1024];
}page_table_t;

/* PD - Page Directory; 1024 PDE - Entrys 
 * Each PDE must have some flags, for example: 
 * bit 0 enabled (present - must be 1 to reference a page table).
 * This kind of information is used by the CPU.
 * That's why we have 'tablesPhysical' for this and tables just
 * for index. 
 * Problem: If we use 'tables' to put all necessary flags, we 
 * can't find the 'page table' because of the flag. So:
 * 'tables' DO NOT use any kind of flags.
 * 'tablesPhysical' USES flags for the CPU.
 */
typedef struct page_directory{
        page_table_t * tables[1024];
        u32int tablesPhysical[1024];
        u32int phy_addr;
}page_directory_t;


page_t * get_page(u32int address,int make,page_directory_t * page_dir);
void enable_paging();
void load_page_dir(page_directory_t *dir);
void page_fault();
void show_page(page_t * page, u32int page_va, page_directory_t * page_dir);

#define PT_CREATE       1 /* Create the page table if there isn't */
#define PAGE_READ       0 
#define PAGE_WRITE      1 
#define PAGE_USER       0 
#define PAGE_KERNEL     1

#define I86_PTE_PRESENT         0x1,          
#define I86_PTE_WRITABLE        0x2,          
#define I86_PTE_USER            0x4,          
#define I86_PTE_PWT             0x8,          
#define I86_PTE_NOT_CACHEABLE   0x10,         
#define I86_PTE_ACCESSED        0x20,         
#define I86_PTE_DIRTY           0x40,         
#define I86_PTE_PAT             0x80,         
#define I86_PTE_CPU_GLOBAL      0x100,        
#define I86_PTE_LV4_GLOBAL      0x200,        
#define I86_PTE_FRAME           0x7FFFF000   

#define I86_PDE_PRESENT         0x1,          
#define I86_PDE_WRITABLE        0x2,          
#define I86_PDE_USER            0x4,          
#define I86_PDE_PWT             0x8,          
#define I86_PDE_NOT_CACHEABLE   0x10,         
#define I86_PDE_ACCESSED        0x20,         
#define I86_PDE_PAT             0x80,         
#define I86_PDE_CPU_GLOBAL      0x100,        
#define I86_PDE_LV4_GLOBAL      0x200,        
#define I86_PDE_FRAME           0x7FFFF000   


