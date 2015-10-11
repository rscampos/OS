#include "common.h"
#include "vfs.h"

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
        printf("pathname:%s\n", pathname);
        return -1;
}
