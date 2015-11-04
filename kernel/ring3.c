#include "common.h"

void never_call(){

        printf("Hello from ring 3\n");
}

void ring3_test(){
        asm volatile("  \ 
                        cli; \ 
                        mov $0x23, %ax; \ 
                        mov %ax, %ds; \ 
                        mov %ax, %es; \ 
                        mov %ax, %fs; \ 
                        mov %ax, %gs; \ 
                        mov %esp, %eax; \ 
                        pushl $0x23; \ 
                        pushl %eax; \ 
                        pushf; \ 
                        popl %eax; \
                        or $0x200, %eax; \
                        pushl %eax; \
                        pushl $0x1b; \ 
                        push $1f; \ 
                        iret; \ 
                        1: \ 
                        mov $0x31, %eax; \
                        mov $0x33, %ebx; \
                        mov $0x07, %ecx; \
                        mov $0xFF, %edx; \
			int $0x80; \
                        ");
        /*
        asm volatile(" \
                        cli; \
                        movl $0x23, %ax; \
                        movl %ax, %ds; \
                        movl %ax, %es; \
                        movl %ax, %fs; \
                        movl %ax, %gs; \
                        pushl $0x23; \
                        pushl %esp; \
                        pushfl; \
                        pushl $0x1B; \
                        pushl $1f; \
                        iret; \
                        1: \
                        sti; \
                        xor %eax, %eax; \
                        ");
        */
}
