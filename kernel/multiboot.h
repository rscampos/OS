#include "common.h"
/*
struct multiboot_info_struct{
        u32int        m_flags;
        u32int        m_memoryLo;
        u32int        m_memoryHi;
        u32int        m_bootDevice;
        u32int        m_cmdLine;
        u32int        m_modsCount;
        u32int        m_modsAddr;
        u32int        m_syms0;
        u32int        m_syms1;
        u32int        m_syms2;
        u32int        m_mmap_length;
        u32int        m_mmap_addr;
        u32int        m_drives_length;
        u32int        m_drives_addr;
        u32int        m_config_table;
        u32int        m_bootloader_name;
        u32int        m_apm_table;
        u32int        m_vbe_control_info;
        u32int        m_vbe_mode_info;
        u16int        m_vbe_mode;
        u32int        m_vbe_interface_addr;
        u16int        m_vbe_interface_len;
};
*/

#define AVAILABLE       1
#define RESERVED        2
#define ACPI_RECLAIM    3
#define ACPI_NVS_MEMORY 4

struct multiboot_info_struct{
        u16int        m_memoryLo;
        u16int        m_memoryHi;
        u16int        m_mmap_length;
        u16int        m_mmap_addr;
};

struct memory_map_struct{
        u32int        m_base_address;
        u32int        m_unlabeled1;
        u32int        m_size;
        u32int        m_unlabeled2;
        u32int        m_type;
};

typedef struct multiboot_info_struct multiboot_info_t;
typedef struct memory_map_struct memory_map_t;
