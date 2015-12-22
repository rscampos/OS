#include "paging.h"
#include "pmm.h"
#include "common.h"
#include "isr.h"
#include "vfs.h"
#include "process.h"
/* remove a process from the list */

extern task_struct_t *head;

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
