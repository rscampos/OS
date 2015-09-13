#include "common.h"

void outb(u16int port, u8int val){
        asm("out %%al, %%dx;"::"a"(val), "d"(port));
}

u8int in(u16int port){
        u8int val;
        asm("in %%dx,%%al;": "=a"(val) : "d"(port));
        return val;
}
