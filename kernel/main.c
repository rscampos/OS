// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 

#include "common.h"
#include "monitor.h"
#include "vfs.h"

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

        //printf("addr:0x%x\n",addr_k);
        //puts("done.\n");
        //
        pathname = "/ROOT/TEMP.TXT";
        printf("Reading %s\n",pathname);
        node = open_vfs(pathname);
        read_vfs(node,ptr,512);
        printf("Conteudo:%s\n",ptr);
        return 1;
}
