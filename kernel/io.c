#include "common.h"

void outb(u16int port, u8int val){
        asm("out %%al, %%dx;"::"a"(val), "d"(port));
}

void outw(u16int port, u16int val){
        __asm __volatile("outw %0, %w1" : : "a" (val), "Nd" (port));
}

void outl(u16int port, u32int val){
        __asm __volatile("outl %0, %w1" : : "a" (val), "Nd" (port));
}

u8int in(u16int port){
        u8int val;
        asm("in %%dx,%%al;": "=a"(val) : "d"(port));
        return val;
}

u8int inl(u16int port){
        u32int val;
        __asm __volatile("inl %w1, %0" : "=a" (val) : "Nd" (port));
        return val;
}
