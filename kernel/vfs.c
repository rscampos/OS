#include "common.h"
#include "vfs.h"

extern vfs_node_t *root_fs;

int read_vfs(vfs_node_t *node, void *buffer, u32int size){
        if(node->read != 0)
                node->read(node, buffer, size);
        else
                return -1;
}

int write_vfs(vfs_node_t *node, const void *buffer, u32int size){
        if(node->write != 0)
                node->write(node, buffer, size);
        else
                return -1;
}

int close_vfs(vfs_node_t *node){
        if(node->close != 0)
                node->close(node);
        else
                return -1;
}

vfs_node_t * open_vfs(const char *pathname){
        vfs_node_t * temp;
        temp = root_fs;
        int count_level=0,i;
        for(i=1;i<strlen(pathname);i++)

                if(pathname[i]=='/')
                        count_level++;


        printf("pathname:%s (%d)\n", pathname, count_level);
        while(temp){
                printf("Found:%s\n",temp->filename);
                temp = temp->next;
        }





        return -1;
}
