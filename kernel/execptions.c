
#include "common.h"
#include "isr.h"

void debug(registers_t regs){
        puts("[Exceptions]: Int1.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}


void int3(registers_t regs){
        puts("[Exceptions]: Int3.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}

void double_fault(registers_t regs){
        printf("[Exceptions]: double fault:%d\n",regs.int_no);
        printf("[Exceptions]: double fault:0x%x\n",regs.eip);
        asm("cli");
        asm("hlt");
}

void divide_error(registers_t regs){
        puts("[Exceptions]: divide_error\n");
        asm("cli");
        puts("[Exceptions]: Do something\n");
        asm("hlt");
}

void general_protection(registers_t regs){
        puts("[Exceptions]: general_protection \n");
        asm("cli");
        puts("[Exceptions]: Do something\n");
        asm("hlt");
}


void init_exceptions(){
        register_interrupt_handler(ISR0,&divide_error);
        
        register_interrupt_handler(ISR1,&debug);
        /*
        register_interrupt_handler(ISR2,&nmi);
        */
        register_interrupt_handler(ISR3,&int3);
        /*
        register_interrupt_handler(ISR6,&invalid_op);
        register_interrupt_handler(ISR7,&device_not_available);
        */
        register_interrupt_handler(ISR8,&double_fault);
        /*
        register_interrupt_handler(ISR9,&coprocessor_segment_overrun);
        register_interrupt_handler(ISR10,&invalid_TSS);
        register_interrupt_handler(ISR11,&segment_not_present);
        register_interrupt_handler(ISR12,&stack_segment);
        */
        register_interrupt_handler(ISR13,&general_protection);
        /*
        register_interrupt_handler(ISR14,&page_fault);
        register_interrupt_handler(ISR15,&spurious_interrupt_bug);
        register_interrupt_handler(ISR16,&coprocessor_error);
        register_interrupt_handler(ISR17,&alignement_check);
        register_interrupt_handler(ISR18,&machine_check);
        */
}

