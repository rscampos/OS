#include "common.h"
#include "descriptor_table.h"
#include "isr.h"

gdt_entry_t     gdt_table[6];
gdt_ptr_t       gdt_ptr;
idt_entry_t     idt_table[256];
idt_ptr_t       idt_ptr;
tss_entry_t     tss_entry;

void idt_set_gate(int num, u32int handler){

        idt_table[num].handler_high     = (handler & 0xFFFF0000) >> 16;
        idt_table[num].handler_low      = (handler & 0xFFFF);

        idt_table[num].segment_selector = 0x8;
        idt_table[num].reserved         = 0x0;

        /* P|DPL|0 1110 - interrupt gate */
        idt_table[num].flags            = 0x8E;
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
 
        idt_set_gate(0,(u32int)isr0);
        idt_set_gate(1,(u32int)isr1);
        /* idt_set_gate(2,(u32int)isr2); */
        idt_set_gate(3,(u32int)isr3);
        /*
        idt_set_gate(4,(u32int)isr4);
        idt_set_gate(5,(u32int)isr5);
        idt_set_gate(6,(u32int)isr6);
        idt_set_gate(7,(u32int)isr7);
        */
        idt_set_gate(8,(u32int)isr8);
        /*
        idt_set_gate(9,(u32int)isr9);
        */
        idt_set_gate(10,(u32int)isr10);
        /*
        idt_set_gate(11,(u32int)isr11);
        idt_set_gate(12,(u32int)isr12);
        */
        idt_set_gate(13,(u32int)isr13);
        idt_set_gate(14,(u32int)isr14);
        /*
        idt_set_gate(15,(u32int)isr15);
        idt_set_gate(16,(u32int)isr16);
        idt_set_gate(17,(u32int)isr17);
        idt_set_gate(18,(u32int)isr18);
        */
        idt_set_gate(32,(u32int)irq0);
        idt_set_gate(33,(u32int)irq1);
        idt_set_gate(38,(u32int)irq6);
}

void init_idt(){

        idt_ptr.idt_base        = (u32int) &idt_table;
        idt_ptr.limit           = sizeof(idt_entry_t)*256 - 1;
        
        /* Remap the irq table.
         */
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
        gdt_set_gate(1,0,0x9A,0xFFFFFF,0xCF);    /* 0x08 - Kernel - code segment */
        gdt_set_gate(2,0,0x92,0xFFFFFF,0xCF);    /* 0x10 - Kernel - data segment */
        gdt_set_gate(3,0,0xFA,0xFFFFFF,0xCF);    /* 0x18 - User   - code segment */
        gdt_set_gate(4,0,0xF2,0xFFFFFF,0xCF);    /* 0x20 - User   - data segment */
        write_tss(5, 0x10, 0x9000);             /* 0x28 - TSS                   */

        gdt_install((u32int)&gdt_ptr);
        tss_install();
}

void write_tss(u32int num, u16int ss0, u32int esp0){
        u32int base, limit;
        
        base    = &tss_entry;
        //limit   = base + sizeof(tss_entry);
        limit   = sizeof(tss_entry_t) - 1;

        gdt_set_gate(num, base, 0x89, limit, 0x0); /* sets for ring0 */

        memset(&tss_entry, 0x0, sizeof(tss_entry));
        
        tss_entry.ss0   = ss0;  /* kernel stack segment */
        tss_entry.esp0  = esp0; /* kernel stack pointer */

        /* We are setting all segments cs, ss, ds, es, fs and gs. 
         * All segment are with RPL (Requested Privilege Level) to 3,
         * which means that TSS can be used to switch from ring3 to ring0.
         */
        tss_entry.cs    = 0x08; 
        tss_entry.ss    = 0x10; 
        tss_entry.ds    = 0x10; 
        tss_entry.es    = 0x10; 
        tss_entry.fs    = 0x10; 
        tss_entry.gs    = 0x10; 
        
}

void init_descriptor_tables(){
        init_gdt();
        init_idt();
}
