#include "common.h"

/* This GDT struct gives information about the memory to the CPU.
 * The attribute 'packet' tells to GCC not to change any of the
 * aligment in the structure.
 */
struct gdt_entry_struct{
        u16int  limit_low;
        u16int  base_low;
        u8int   base_middle;
        u8int   access;
        u8int   granularity;
        u8int   base_high;
}__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

/* This struct stores the location and the size of gdt_entry_struct.
 * This struct are going to be passed to GDTR (register).
 */
struct gdt_ptr_struct{
        u16int  limit;
        u32int  gdt_base;
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

/* This IDT struct gives information about the interrupts to the CPU.
*/
struct idt_entry_struct{
        u16int  handler_low;
        u16int  segment_selector;
        u8int   reserved;
        u8int   flags;
        u16int  handler_high;
}__attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

/* This struct stores the location and the size of gdt_entry_struct.
 * This struct are going to be passed to GDTR (register).
 */
struct idt_ptr_struct{
        u16int  limit;
        u32int  idt_base;
}__attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;


/* 
 * Task State Segment (TSS) struct
 */
struct tss_entry_struct
{
        u32int prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
        u32int esp0;       // The stack pointer to load when we change to kernel mode.
        u32int ss0;        // The stack segment to load when we change to kernel mode.
        u32int esp1;       // Unused...
        u32int ss1;
        u32int esp2;
        u32int ss2;
        u32int cr3;
        u32int eip;
        u32int eflags;
        u32int eax;
        u32int ecx;
        u32int edx;
        u32int ebx;
        u32int esp;
        u32int ebp;
        u32int esi;
        u32int edi;
        u32int es;         // The value to load into ES when we change to kernel mode.
        u32int cs;         // The value to load into CS when we change to kernel mode.
        u32int ss;         // The value to load into SS when we change to kernel mode.
        u32int ds;         // The value to load into DS when we change to kernel mode.
        u32int fs;         // The value to load into FS when we change to kernel mode.
        u32int gs;         // The value to load into GS when we change to kernel mode.
        u32int ldt;        // Unused...
        u16int trap;
        u16int iomap_base;
}__attribute__((packed));
typedef struct tss_entry_struct tss_entry_t;

void init_descriptor_tables();

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* Lets us access our ASM functions from our C code. */
extern void gdt_install(u32int);
extern void tss_install();
extern void idt_install(u32int);

// Internal function prototypes.
void init_gdt();
void init_idt();
void write_tss(u32int,u16int,u32int);
void gdt_set_gate(int,u32int,u8int,u32int,u8int);
void idt_set_gate(int,u32int,u16int,u8int);
