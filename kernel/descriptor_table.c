#include "common.h"
#include "descriptor_table.h"
#include "isr.h"

gdt_entry_t     gdt_table[6];
gdt_ptr_t       gdt_ptr;
idt_entry_t     idt_table[256];
idt_ptr_t       idt_ptr;
tss_entry_t     tss_entry;

void idt_set_gate(int num, u32int handler, u16int sel, u8int flags){

        idt_table[num].handler_high     = (handler & 0xFFFF0000) >> 16;
        idt_table[num].handler_low      = (handler & 0xFFFF);

        idt_table[num].segment_selector = sel;
        idt_table[num].reserved         = 0x0;

        /* P|DPL|0 1110 - interrupt gate */
        idt_table[num].flags            = flags | 0x60;
}

void gdt_set_gate(int num, u32int base, u8int access, u32int limit, u8int granularity){

        gdt_table[num].base_low      = (base & 0xFFFF);
        gdt_table[num].base_middle   = (base >> 16) & 0xFF;
        gdt_table[num].base_high     = (base >> 24);

        gdt_table[num].limit_low     = limit & 0xFFFF;
        gdt_table[num].granularity   = (limit >> 16) & 0x0F;
        gdt_table[num].granularity   |= granularity & 0xF0;

        gdt_table[num].access        = access;
}
void init_gates(){
 
        idt_set_gate(0,(u32int)isr0,0x8,0x8E);
        idt_set_gate(1,(u32int)isr1,0x8,0x8E);
        idt_set_gate(3,(u32int)isr3,0x8,0x8E);
	idt_set_gate(8,(u32int)isr8,0x8,0x8E);
	idt_set_gate(10,(u32int)isr10,0x8,0x8E);
	idt_set_gate(13,(u32int)isr13,0x8,0x8E);
        idt_set_gate(14,(u32int)isr14,0x8,0x8E);
	idt_set_gate(32,(u32int)irq0,0x8,0x8E);
        idt_set_gate(33,(u32int)irq1,0x8,0x8E);
        idt_set_gate(38,(u32int)irq6,0x8,0x8E);
       	idt_set_gate(128,(u32int)isr128,0x8,0xEE);
}

void init_idt(){

        idt_ptr.idt_base        = (u32int) &idt_table;
        idt_ptr.limit           = sizeof(idt_entry_t)*256 - 1;
        
        /* Remap the irq table */
        outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);      /* PIC inicialization - Master */
        outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);      /* PIC inicialization - Slave */
        outb(PIC1_DATA, PIC1_OFFSET);                   /* ICW2 - IRQ0 fires 'int 32' - Master */
        outb(PIC2_DATA, PIC2_OFFSET);                   /* ICW2 - IRQ8 fires 'int 40' - Slave */
        outb(PIC1_DATA, 0x04);                          /* ICW3 - slave PIC in IRQ2 - 0x04 (0100) */
        outb(PIC2_DATA, 0x02);                          /* ICW3 - master PIC in slave pic IRQ1 */
        outb(PIC1_DATA, ICW4_8086);                     /* ICW4 - */
        outb(PIC2_DATA, ICW4_8086);                     /* ICW4 - */
        outb(0x21, 0x0);                                /* ICW3 - */
        outb(0xA1, 0x0);                                /* ICW3 - */
        
        init_gates();

        idt_install((u32int)&idt_ptr);
}

void init_gdt(){

        gdt_ptr.gdt_base        = (u32int) &gdt_table;
        gdt_ptr.limit           = sizeof(gdt_entry_t)*6 - 1;

        gdt_set_gate(0,0,0,0,0);                /* 0x00 - Null segment          */
        gdt_set_gate(1,0,0x9A,0xFFFFFF,0xCF);	/* 0x08 - Kernel - code segment */
        gdt_set_gate(2,0,0x92,0xFFFFFF,0xCF);	/* 0x10 - Kernel - data segment */
        gdt_set_gate(3,0,0xFA,0xFFFFFF,0xCF);	/* 0x18 - User   - code segment */
        gdt_set_gate(4,0,0xF2,0xFFFFFF,0xCF);	/* 0x20 - User   - data segment */
        write_tss(5, 0x10, 0x9000);		/* 0x28 - TSS                   */

        gdt_install((u32int)&gdt_ptr);
        tss_install();
}

void write_tss(u32int num, u16int ss0, u32int esp0){
        u32int base, limit;
        
        base    = &tss_entry;
        limit   = sizeof(tss_entry_t) - 1;

        gdt_set_gate(num, base, 0xE9, limit, 0x0);

        memset(&tss_entry, 0x0, sizeof(tss_entry));
        
        tss_entry.ss0   = ss0;  /* kernel stack segment */
        tss_entry.esp0  = esp0; /* kernel stack pointer */
}

void init_descriptor_tables(){
        init_gdt();
        init_idt();
}
