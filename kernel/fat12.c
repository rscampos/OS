#include "common.h"
#include "vfs.h"
#include "fat12.h"

int read_fs_fat12(vfs_node_t *node, void *buffer, u32int size){
        printf("Read callback called.\n");
}

int write_fs_fat12(vfs_node_t *node, void *buffer, u32int size){
        printf("Read callback called.\n");
}

int close_fs_fat12(vfs_node_t *node){
        printf("Close callback called.\n");
}

