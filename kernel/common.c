#include "common.h"

void memset(u8int *s, s8int c, int sz){
        int i=0;
        for(i;i<sz;i++){
                *s=c;
                s++;
        }
}

/* from freebsd */
int strcmp(const char *s1, const char *s2)
{
	for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++)
					;
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

u32int set_flags(u32int value, u32int flags){
        return value | flags;
}
