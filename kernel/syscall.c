#include "paging.h"
#include "pmm.h"
#include "common.h"
#include "isr.h"
#include "vfs.h"
#include "process.h"
/* remove a process from the list */

extern task_struct_t *head;

/*
static void *syscall_table[3] = 
{
	&printf,
};
*/

void do_switch(registers_t regs){
	printf("[switch]");
	switch_task(regs);
	return;
}

void do_exit(registers_t regs){
	printf("[exit]");
	head->finished = 1;
	switch_task(regs);
	return;
}

void do_write(registers_t regs){
	printf("[write:%s]",regs.ecx);
	return;
}

void syscall_handler(registers_t regs){
	/* swtich sync */
	if(regs.eax == 0)
		do_switch(regs);

	/* exit */
	if(regs.eax == 1)
		do_exit(regs);

	/* write */
	if(regs.eax == 4)
		do_write(regs);

	return;
}

void init_syscall(){
	register_interrupt_handler(0x80,&syscall_handler);
}
