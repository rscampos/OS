
#include "common.h"
#include "isr.h"

/* 0 - #DE */
void divide_error(registers_t regs){
        puts("[Exceptions]: divide_error\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);
        
        asm("cli");
        asm("hlt");
}

/* 1 - #DB */
void debug(registers_t regs){
        puts("[Exceptions]: Int1.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}

/* 2 - #?
void nmi(registers_t regs){
        puts("[Exceptions]: nmi.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 3 - #BP int3(0xcc) */
void int3(registers_t regs){
        puts("[Exceptions]: Int3.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}

/* 4 - #OF
void overflow(registers_t regs){
        puts("[Exceptions]: Overflow.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 5 - #BR
void bound_range_exceeded(registers_t regs){
        puts("[Exceptions]: Bound Range Exceeded.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 6 - #UD
void invalid_op(registers_t regs){
        puts("[Exceptions]: Invalid Opcode.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 7 - #NM
void device_not_available(registers_t regs){
        puts("[Exceptions]: Device not available.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 8 - #DF */
void double_fault(registers_t regs){
        printf("[Exceptions]: Int num:%d\n",regs.int_no);
        printf("[Exceptions]: Error code:0x%x\n",regs.err_code);
        printf("[Exceptions]: EIP:0x%x\n",regs.eip);
        asm("cli");
        asm("hlt");
}

/* 9 - #?
void coprocessor_segment_overrun(registers_t regs){
        puts("[Exceptions]: Coprocessor segment overrun.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 10 - #TS */
void invalid_TSS(registers_t regs){
        printf("[Exceptions]: invalid TSS:%d\n",regs.int_no);
        printf("[Exceptions]: invalid TSS:0x%x\n",regs.eip);
        asm("cli");
        asm("hlt");
}

/* 11 - #NP
void segment_not_present(registers_t regs){
        puts("[Exceptions]: Coprocessor segment overrun.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 12 - #SS
void stack_segment(registers_t regs){
        puts("[Exceptions]: Stack segment.\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 13 - #GP */
void general_protection(registers_t regs){
        printf("[Exceptions]: Num:%d\n",regs.int_no);
        printf("[Exceptions]: Error code:0x%x\n",regs.err_code);
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}

/* 15
void spurious_interrupt_bug(registers_t regs){
        puts("[Exceptions]: Purious interrupt bug\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 16
void coprocessor_error(registers_t regs){
        puts("[Exceptions]: Coprocessor error\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 17
void alignement_check(registers_t regs){
        puts("[Exceptions]: alignement_check\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/

/* 18
void machine_check(registers_t regs){
        puts("[Exceptions]: Machine check\n");
        printf("[Exceptions]: EIP=>0x%x\n",regs.eip);

        asm("cli");
        asm("hlt");
}
*/


void int80(registers_t regs){
	printf("Syscall fired!\n");
	printf("FUNC (EAX): 0x%x\n",regs.eax);
	printf("(EBX): 0x%x\n",regs.ebx);
	printf("(ECX): 0x%x\n",regs.ecx);
	printf("(EDX): 0x%x\n",regs.edx);
}

void init_exceptions(){
        register_interrupt_handler(ISR0,&divide_error);
        register_interrupt_handler(ISR1,&debug);
        //register_interrupt_handler(ISR2,&nmi);
        register_interrupt_handler(ISR3,&int3);
        //register_interrupt_handler(ISR4,&overflow);
        //register_interrupt_handler(ISR5,&bound_range_exceeded);
        //register_interrupt_handler(ISR6,&invalid_op);
        //register_interrupt_handler(ISR7,&device_not_available);
        register_interrupt_handler(ISR8,&double_fault);
        //register_interrupt_handler(ISR9,&coprocessor_segment_overrun);
        register_interrupt_handler(ISR10,&invalid_TSS);
        //register_interrupt_handler(ISR11,&segment_not_present);
        //register_interrupt_handler(ISR12,&stack_segment);
        register_interrupt_handler(ISR13,&general_protection);
        /* register_interrupt_handler(ISR14,&page_fault); */
        //register_interrupt_handler(ISR15,&spurious_interrupt_bug);
        //register_interrupt_handler(ISR16,&coprocessor_error);
        //register_interrupt_handler(ISR17,&alignement_check);
        //register_interrupt_handler(ISR18,&machine_check);
        register_interrupt_handler(0x80,&int80);
}

