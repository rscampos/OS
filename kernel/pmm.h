
#include "common.h"
#include "multiboot.h"

void pmm_memory_map(multiboot_info_t* bootinfo);

int pmm_first_free();

u32int pmm_used_memory();

typedef u32int physical_addr;
