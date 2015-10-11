struct vfs_node;

/* typedef to define the type of callbacks */
typedef int (*read_vfs_t)(struct vfs_node *,void *, u32int);
typedef int (*write_vfs_t)(struct vfs_node *,const void *, u32int);
typedef int (*close_vfs_t)(struct vfs_node *);

typedef struct vfs_node{
        /* For FAT12 - 8 name + 3 ext + 2 extras (NULL/.) */
        char            filename[13];
        u32int          inode; 
        u8int           attr;
        u32int          size;
        u16int          first_cluster;
        u16int          current_cluster;
        u16int          offset;
        read_vfs_t      read;
        write_vfs_t     write;
        close_vfs_t     close;
        /* TODO - time info */
}vfs_node_t;

int read_vfs(vfs_node_t *node, void *buffer, u32int size);
int write_vfs(vfs_node_t *node, const void *buffer, u32int size);
int close_vfs(vfs_node_t *node);
vfs_node_t * open_vfs(const char *pathname);
