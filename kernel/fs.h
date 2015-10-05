typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);

typedef u32int (*read_type_t)(struct fs_node*,u32int,u32int,u8int*);
typedef u32int (*write_type_t)(struct fs_node*,u32int,u32int,u8int*);

typedef struct dirent * (*readdir_type_t)(struct fs_node*,u32int);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*,char *name);


/*  One of these is returned by the readdir call, according to POSIX.
*/
struct dirent {
        char name[128];         /* Filename. */
        u32int ino;             /* Inode number. Required by POSIX. */
};

typedef struct fs_node
{
        char name[128];         /* The filename. */
        u32int mask;            /* The permissions mask. */
        u32int uid;             /* The owner user. */
        u32int gid;             /* The owner group. */
        u32int length;          /* Size of the file, in bytes. */
        u32int flags;           /* Includes the node type (Directory, file etc). */
        u32int inode;           /* Provides a way for a filesystem to identify files. */
        u32int impl;            /* An implementation-defined number. */
        u32int position;        /* Offset inside the cluster. */
        u32int ccluster;        /* Current cluster. */
        read_type_t read;       /* Typedefs are just function pointers. */
        write_type_t write;
        open_type_t open;
        close_type_t close;
        readdir_type_t readdir; /* Returns the n'th child of a directory. */
        finddir_type_t finddir; /* Try to find a child in a directory by name. */
        struct fs_node *ptr;    /* Used by mountpoints and symlinks. */
}fs_node_t;   

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08     /* Is the file an active mountpoint? */


extern fs_node_t *fs_root;      /* The root of the filesystem. */

/* Standard read/write/open/close functions. Note that these are all suffixed with
 * _fs to distinguish them from the read/write/open/close which deal with file descriptors
 * not file nodes.
 */
u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer);
u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer);
void open_fs(fs_node_t *node, u8int read, u8int write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, u32int index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);
