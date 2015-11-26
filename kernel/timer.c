#include "common.h"
#include "isr.h"
//#include "process.h"

u32int tick;

static void timer_callback(registers_t regs){
        tick++;
	switch_task(regs);
	return;
}

void init_timer(int frequency){
        int divisor;
        u8int l, h;
	tick = 0;
        register_interrupt_handler(IRQ0,&timer_callback);
        
        divisor = 1193180 / frequency;

        /* Sending 0x36 to the command port (0x43) sets the
         * PIT to repeating mode(so that when the divisor
         * counter reaches zero it's automatically refreshed).
         */
        outb(0x43, 0x36);

        l = (u8int) (divisor & 0xFF);
        h = (u8int) ((divisor >> 8) & 0xFF);

        outb(0x40,l);
        outb(0x40,h);
}
