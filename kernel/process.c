#include "paging.h"
#include "pmm.h"
#include "common.h"
#include "isr.h"
#include "vfs.h"
#include "process.h"

extern page_directory_t *kernel_directory;
static int pid;
task_struct_t *start, *head;
extern u32int tick;
static u32int task_start;
static u32int task_c;

page_directory_t * clone_page_dir(page_directory_t *pd){
	page_directory_t *new_pd;
	page_directory_t *temp_pd;
	page_t *page_temp;
	u32int *temp;
	int pde, pte;

	//printf("Cloning PD:0x%x\n", pd);	
	new_pd = (page_directory_t*) kmalloc_a(sizeof(page_directory_t), 1);

	/* get the frame addr. */
	page_temp = get_page((u32int *)new_pd->tablesPhysical, 1, kernel_directory);

	memcpy(new_pd, pd, sizeof(page_directory_t));

	new_pd->phy_addr = page_temp->frame_address << 12;

	return new_pd;
};

void init_process(){
	pid = 0;
	task_start = 0;
	task_c = 0;
	start = head;
	printf("Init process!\n");
}

task_struct_t * make_process(){
	task_struct_t * proc;
	
	proc = kmalloc(sizeof(task_struct_t));
	
	if(!start){
		start = proc;
		head  = proc;
	}else{
		head->next = proc;
		head = proc;
	}
	head->next = start;

	return proc;
}

int create_process(const char *pathname){
	vfs_node_t * node;
	u8int *ptr;
	task_struct_t * process;
	page_t * temp;
	u32int virt_addr, esp_addr;

	/* Putting the process in the linked list */
	process = make_process();
	process->pid = ++pid;
	//process->finished = 0;

	/* Creating a PD for the new process */
	process->pd = clone_page_dir(kernel_directory);
	
	/* Copying the program imagem from disk to memory */
	node = open_vfs(pathname);
	
	if(node == -1)
		return -1;

        read_vfs(node, ptr, 512);
	load_page_dir(process->pd);
	virt_addr = 0x0804A000;
	esp_addr = 0xBFFFA000;
	process->eip = virt_addr;
	process->esp = esp_addr;
	
	/* Create stack */
	temp = get_page(esp_addr, 1, process->pd);
	pmm_alloc_frame(temp, PAGE_USER, PAGE_WRITE);
	
	/* machine code */
	temp = get_page(virt_addr, 1, process->pd);
	pmm_alloc_frame(temp, PAGE_USER, PAGE_WRITE);
	memcpy(virt_addr, ptr, node->size);
	
	load_page_dir(kernel_directory);
	close_vfs(node);

	/*
	printf(" [+] Process %s created!\n", pathname);
	printf(" [-] Info:\n");
	printf("   [-] PID:%d\n", process->pid);
	printf("   [-] PD:0x%x\n", process->pd);
	*/
	
	return process->pid;
}


void switch_task(registers_t regs){

	if(!task_start) return;
	
	/* salva contexto do processo que foi trocado */
	head->esp = regs.useresp;
	head->eip = regs.eip;

	/* infos */
	//printf("[%d EIP:0x%x EAX:0x%x 0x%x 0x%x 0x%x ESI:0x%x 0x%x EBP:0x%x ESP:0x%x]\n", head->pid, regs.eip, regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi, regs.ebp, head->esp);

	/* coloca o novo processo para ser executado */
	while(1){
		head = head->next;
		if(head->finished == 0){
			printf("[next:%d]", head->pid);
			break;
		}
	}

	load_page_dir(head->pd);
	asm volatile ("cli; \
		mov $0x23, %%ax; \
		mov %%ax, %%ds; \
		mov %%ax, %%es; \
		mov %%ax, %%fs; \
		mov %%ax, %%gs; \
		movl %0, %%ebx; \
		pushl $0x23; \
		pushl %%ebx; \
		pushf; \
		popl %%eax; \
		or $0x200, %%eax; \
		pushl %%eax; \
		pushl $0x1b; \
		pushl %1; \
		iret;"::"r"(head->esp), "r"(head->eip));
	/*
	printf("c:%d [%d EIP:0x%x EAX:0x%x 0x%x 0x%x 0x%x ESI:0x%x 0x%x EBP:0x%x ESP:0x%x]\n", task_c, head->pid, regs.eip, regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi, regs.ebp, regs.esp);
*/	
}

void exec_process(){

	printf("Executing...:(start pid:%d)\n",head->pid);
	
	// Start executing the first process...
	asm("cli");
	task_start = 1;

	// ---- create a function with this ----
	load_page_dir(head->pd);
	
	asm volatile ("cli; \
		movl %0, %%eax; \
		pushl $0x23; \
		pushl %%eax; \
		mov $0x23, %%ax; \
		mov %%ax, %%ds; \
		mov %%ax, %%es; \
		mov %%ax, %%fs; \
		mov %%ax, %%gs; \
		pushf; \
		popl %%eax; \
		or $0x200, %%eax; \
		pushl %%eax; \
		pushl $0x1b; \
		pushl %1; \
		iret;"::"r"(head->esp), "r"(head->eip));
}

