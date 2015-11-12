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

        //printf("addr:0x%x\n",addr_k);
        //puts("done.\n");
        //

        pathname = "/PROGRAMS/RING3";
//        printf("Reading %s\n",pathname);

        node = open_vfs(pathname);
        read_vfs(node, ptr, 512);
        //write_vfs(node, ptr, 512);

        /* Show the file content */
        /*
        printf("Content: ");
        for(int i=0; i < node->size; i++)
                printf("0x%x ",ptr[i]);
        printf("\n");
        */
        memcpy(0x0804A000, ptr, node->size);
        close_vfs(node);

        /* call ring3 */
        ring3_test();

        return 1;
}
