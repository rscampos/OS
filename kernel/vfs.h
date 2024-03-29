struct vfs_node;

/* typedef to define the type of callbacks */
typedef int (*read_vfs_t)(struct vfs_node *,void *, u32int);
typedef int (*write_vfs_t)(struct vfs_node *,const void *, u32int);
typedef int (*close_vfs_t)(struct vfs_node *);

typedef struct vfs_node{
        /* For FAT12 - 8 name + 3 ext + 2 extras (NULL/.) */
        char            filename[13];
        u16int          inode; 
        u8int           attr;
        u32int          size;
        u16int          first_cluster;
        u16int          current_cluster;
        u16int          offset;
        read_vfs_t      read;
        write_vfs_t     write;
        close_vfs_t     close;
        /* TODO - time info */
        struct vfs_node *next;
        struct vfs_node *subdir;
}vfs_node_t;

int read_vfs(vfs_node_t *, void *, u32int);
int write_vfs(vfs_node_t *, const void *, u32int);
int close_vfs(vfs_node_t *);
vfs_node_t * open_vfs(const char *);

#define VFS_DIRECTORY    0X10
#define VFS_FILE         0X20
