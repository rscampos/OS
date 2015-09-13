#include "common.h"
#include "isr.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(int irq, isr_t handler){
        interrupt_handlers[irq] = handler;
}

void isr_handler(registers_t regs){
        if(interrupt_handlers[regs.int_no] != 0){
                isr_t handler = interrupt_handlers[regs.int_no];
                handler(regs);
        }
}

void irq_handler(registers_t regs){

        if(regs.int_no >= 40)
                outb(PIC2_COMMAND,0x20);        /* send EOI to the slave*/
        outb(PIC1_COMMAND,0x20);                /* send EOI to the master*/

        if(interrupt_handlers[regs.int_no] != 0){
                isr_t handler = interrupt_handlers[regs.int_no];
                handler(regs);
        }
}

