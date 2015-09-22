// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 

#include "common.h"
#include "monitor.h"
int main()
{
        int sum, mul, num1, num2,size;
        num1=-100;
        num2=60;
        char interger[11];
        char buffer[50];
        u32int *ptr;
        u32int *do_page_fault;
        u32int addr;
        int i;
        u32int addr_k;
        char *banner = "Welcome to my operation system!\n";
               
        //for(i=0;i<=8192;i++){
        for(i=0;i<=100;i++){
                size = 512; // 1MB
                addr_k = kmalloc(size);
                if(i==0) printf("addr:0x%x\n",addr_k);
                memset(addr_k,0x41, size);
        }

        printf("addr:0x%x\n",addr_k);
        puts("done.\n");
 
        return 1;
}
