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

vfs_node_t * open_vfs(const char *path){
        vfs_node_t * temp;
        char *search=0;
	char *pathname;
        temp = root_fs;
        int i,size_path;
	
	size_path = strlen(path);
	pathname  = kmalloc(size_path + 1); /* +1 for NULL terminator */
	memcpy(pathname, path, size_path + 1);

	/* Iteration over the string (path) */
        for(i=0; i <= size_path; i++){
                if(pathname[i]=='/' || pathname[i]=='\0'){
                        
			pathname[i]=0x00;
			
			/* search points to word (directory or file) */
                        if(search){ 
                                while(temp){
                                        if(strcmp(search,temp->filename) == 0){

						/* file found */
                                                if(i == size_path)
                                                        return temp;
						
						/* subdir found */
                                                temp = temp->subdir;
                                                break;
                                        }
                                        temp = temp->next;
                                }
                        }

                        search = &pathname[i+1];
                }
        }
        
	return -1;
}
