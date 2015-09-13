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
        //puts(banner);
        //        clear_screen();

        //       strcpy(buffer, banner);
        //      puts(buffer);

        
        /*
        addr = 0x30400000+0x1000*1024;
        printf("trying to acess the address 0x%x\n", addr);
        *ptr = (u32int*) addr;
        do_page_fault = *ptr;
        strcpy(do_page_fault, banner);
        puts(do_page_fault);
        puts("done.\n");
        */
        //for(i=0;i<1500;i++){ 
/*
        size = 512;
        addr_k = kmalloc(size);
        memset(addr_k,0x41, size);
 

        size = 512 * 20000*2000;
        addr_k = kmalloc(size);
        memset(addr_k,0x41, size);
  */      
        //}
    //    printf("addr:0x%x\n",addr_k);
        return 1;
}
