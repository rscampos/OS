// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "common.h"
#include "monitor.h"
#include "vfs.h"

int main()
{
        int sum, mul, num1, num2,size;
        int pid;
	char interger[11];
        char buffer[50];
        u8int *ptr;
        u32int *do_page_fault;
        u32int addr;
        int i;
        u32int addr_k;
        vfs_node_t * node;
        char * pathname;
        char *banner = "Welcome to my operation system!\n";
        ptr = kmalloc(512);
        //for(i=0;i<=8192;i++){ - error
        /*
        for(i=0;i<=8191;i++){
                size = 512; // 1MB
                addr_k = kmalloc(size);
                if(i==0) printf("addr:0x%x\n",addr_k);
                memset(addr_k,0x41, size);
        }
        */
	
	/* Create all the process */
	
	pid = create_process("/PROGRAMS/FIRST");
	printf("Process:%d created!\n", pid);
	
	pid = create_process("/PROGRAMS/SECOND");
	printf("Process:%d created!\n", pid);
	
	pid = create_process("/PROGRAMS/THIRD");
	printf("Process:%d created!\n", pid);
	
	/* Start exec all the created process */
        exec_process();
	printf("Testando!!!\n");

        return 1;
}
