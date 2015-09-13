#define VIDMEM 0xB8000
#define CHAR_ATTRIB 0x02
#define COLS 80      
#define LINES 25
#define FIRST_LINE 1
#define LAST_LINE 24

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

typedef int bool;
#define TRUE 1
#define FALSE 0

/* Used for PIC reconfiguration */
#define PIC1            0x20            /* IO base address for master PIC */
#define PIC2            0xA0            /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
#define PIC1_OFFSET     0x20            /* 0x20 - 32. IRQ0 fires int 32 */
#define PIC2_OFFSET     0x28

#define ICW1_ICW4       0x01            /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02            /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04            /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08            /* Level triggered (edge) mode */
#define ICW1_INIT       0x10            /* Initialization - required! */
 
#define ICW4_8086       0x01            /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02            /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08            /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C            /* Buffered mode/master */
#define ICW4_SFNM       0x10            /* Special fully nested (not) */

/* Used for keyboard controller */
#define KB_DATA         0X60            /* Input/Ouput buffer */
#define KB_COMMAND      0X64            /* Status Register */

/* For VMM */
#define PAGE_SIZE       4096
#define PAGE_ALIGN      PAGE_SIZE

/* For PMM */
#define PMM_FRAME_SIZE  4096
#define PMM_FRAME_ALIGN PMM_FRAME_SIZE
