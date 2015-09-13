

#include "common.h"

void memset(u8int *s, s8int c, int sz){
        int i=0;
        for(i;i<sz;i++){
                *s=c;
                s++;
        }
}

u32int set_flags(u32int value, u32int flags){
        return value | flags;
}
