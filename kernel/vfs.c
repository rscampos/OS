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
        int i,s_path;

	pathname = kmalloc(strlen(path));
	memcpy(pathname, path, strlen(path)+1);
        s_path = strlen(pathname) + 2;

        for(i=0;i<s_path;i++){
                if(pathname[i]=='/' || pathname[i]=='\0'){
                        pathname[i]=0x00;
                        if(search){
                                while(temp){
                                        //printf("Search:%s filename:%s\n",search, temp->filename); /* TODO */
                                        if(strcmp(search,temp->filename) == 0){
                                                //printf("Found:%s (i:%d s_path:%d)\n",temp->filename,i,s_path);
                                                if(i+1 == s_path)
                                                        return temp;
                                                temp = temp->subdir; /* enter subdir */
                                                break;
                                        }
                                        temp = temp->next;
                                }
                        }

                        search = &pathname[i+1];
                }
        }
        //printf("File:%s, %s\n",search,temp->filename);

        return -1;
}
