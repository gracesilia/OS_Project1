#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <sys/types.h>
#define TIME_UNIT { volatile unsigned long i; for (i = 0; i < 1000000UL; i++); }
#define CHILD_CPU 1
#define PARENT_CPU 0

struct process {
	char name[32];
	int readytime;
	int executiontime;
	int idx;
	pid_t pid;
};

int proc_assign_cpu(int pid, int core); // Assign process to specific core 
int proc_exec(struct process proc); // Execute the process and return pid 
int blockprocess(int pid); // Set very low priority tp process
int wakeupprocess(int pid); // Set high priority to process

#endif
