#include "common.h"

void memset(u8int *s, s8int c, int sz){
        int i=0;
        for(i;i<sz;i++){
                *s=c;
                s++;
        }
}

int strcmp(const char *s1, const char *s2){
        int i=0, j=0;
        int s_s1 = strlen(s1);
        int s_s2 = strlen(s2);

        
        if(s1[0] != s2[0])
                if(s1[0]>s2[0])
                        return 1;
                else
                        return -1;
                  
        for(i;i<s_s1;i++)
                if(s1[i]!=s2[i])
                        break;

        for(j;j<s_s2;j++)
                if(s2[j]!=s1[j])
                        break;

        return i-j;
}

u32int set_flags(u32int value, u32int flags){
        return value | flags;
}
