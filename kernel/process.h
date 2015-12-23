#include "common.h"

typedef struct regs_struct{
	u32int eip, edi, esi, ebp, esp, eax, ebx, ecx, edx, eflags;
}regs_struct_t;

typedef struct task_struct{
	u32int pid;
	char *name;
	struct task_struct *next;
	struct page_directory_t *pd;
	u32int temp;
	struct regs_struct regs;
	u32int finished;
}task_struct_t;

page_directory_t * clone_page_dir(page_directory_t *);
void init_process();
int create_process(const char *);
void exec_process();
void exit_process();
void switch_task();
