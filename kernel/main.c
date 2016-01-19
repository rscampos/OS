// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "common.h"
#include "monitor.h"
#include "vfs.h"

int main()
{
        int pid;
	/* Create all the process */
		
	pid = create_process("/PROGRAMS/FIRST");
	if(pid != -1)
		printf("Process:%d created!\n", pid);
	
	pid = create_process("/PROGRAMS/SECOND");
	if(pid != -1)
		printf("Process:%d created!\n", pid);
	
	pid = create_process("/PROGRAMS/THIRD");
	if(pid != -1)
		printf("Process:%d created!\n", pid);

	pid = create_process("/PROGRAMS/FOURTH");
	if(pid != -1)
		printf("Process:%d created!\n", pid);

	/* Start exec all the created process */
        exec_process();
	printf("Testando!!!\n");

        return 1;
}
