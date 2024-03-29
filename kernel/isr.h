#include "common.h"

/* Execptions ISR */
#define ISR0 0
#define ISR1 1
#define ISR2 2
#define ISR3 3
#define ISR4 4
#define ISR5 5
#define ISR6 6
#define ISR7 7
#define ISR8 8
#define ISR9 9
#define ISR10 10
#define ISR11 11
#define ISR12 12
#define ISR13 13
#define ISR14 14
#define ISR15 15
#define ISR16 16
#define ISR17 17
#define ISR18 18
#define ISR19 19
#define ISR20 20
#define ISR21 21
#define ISR22 22
#define ISR23 23
#define ISR24 24
#define ISR25 25
#define ISR26 26
#define ISR27 27
#define ISR28 28
#define ISR29 29
#define ISR30 30
#define ISR31 31
#define ISR38 38

/* IRQs Master PIC */
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39

/* IRQs Slave PIC */
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

/* Used for the isr_handler */
struct registers
{
        u32int ds;                                      /* Data segment selector */
        u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* Pushed by pusha */
        u32int int_no, err_code;                        /* Interrupt number and error code (if applicable) */
        u32int eip, cs, eflags, useresp, ss;            /* Pushed by the processor automatically - used by iret */
}__attribute__((packed));
typedef struct registers registers_t;

typedef void (*isr_t)(registers_t); 
void register_interrupt_handler(int irq, isr_t handler);
void irq_handler(registers_t regs);
void isr_handler(registers_t regs);
