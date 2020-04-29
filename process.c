#define _GNU_SOURCE
#include "process.h"
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define GET_TIME 314
#define PRINTK 315

int proc_assign_cpu(int pid, int core){
	if (core > sizeof(cpu_set_t)) {
		fprintf(stderr, "Core index error.");
		return -1;
	}

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
		
	if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) {
		perror("sched_setaffinity error");
		exit(1);
	}

	return 0;
}

int proc_exec(struct process ps){
	int pid = fork();

	if (pid < 0) {
		perror("fork");
		return -1;
	}

	if (pid == 0) {
		
		pid_t pidtmp = getpid();

        long starttime = syscall(334);
        // execute the process
        for (int i=0; i <  ps.executiontime; i++){
            TIME_UNIT
        }
        long endtime = syscall(334);
        syscall(333, pidtmp, starttime, endtime,11111);
        #ifdef DEBUG
        	static const long BASE = 1000000000;
        	fprintf(stderr, "[Project1] %d %ld.%09ld %ld.%09ld\n", pidtmp, starttime / BASE, starttime % BASE, endtime / BASE, endtime % BASE);
        #endif
        exit(0);
    }
	proc_assign_cpu(pid, CHILD_CPU);

	return pid;
}

int blockprocess(int pid){
	struct sched_param param;
	param.sched_priority = 0;

	int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
	
	if (ret < 0) {
		perror("sched_setscheduler error");
		return -1;
	}

	return ret;
}

int wakeupprocess(int pid){
	struct sched_param param;
	
	
	param.sched_priority = 0;
	int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
	
	if (ret < 0) {
		perror("sched_setscheduler error");
		return -1;
	}

	return ret;
}
