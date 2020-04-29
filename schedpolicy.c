#define _GNU_SOURCE
#include "process.h"
#include "schedpolicy.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

/* Last context switch time for 1 scheduling */
/* Current unit time */
/* Index of runningprocess process. -1 if no process runningprocess */
/* Number of finish Process */
int lastcontextswitch, current_time, runningprocess, num_finish_process;


int cmp(const void * a, const void * b){
  struct process *orderA = (struct process *)a;
  struct process *orderB = (struct process *)b;
  if ( orderA->readytime < orderB->readytime ) return -1;
  if ( orderA->readytime > orderB->readytime ) return -1;
  return 0;
}

/* Return index of next process  */
int next_process(struct process *ps, int n, int policy){
	/* Non-preemptive */
	if (runningprocess != -1 && policy == 2) return runningprocess;
	if (runningprocess != -1 && policy == 3) return runningprocess;

	int nextprocess = -1;
	//FIFO
	if (policy == 0) {
		for(int i=0; i<n; i++) {
			if(ps[i].pid == -1 || ps[i].executiontime == 0)
				continue;
			if(nextprocess == -1 || ps[i].readytime < ps[nextprocess].readytime)
				nextprocess = i;
		}
    }

    //RR
	else if (policy == 1) {
		if (runningprocess == -1) {
			for (int i=0; i<n; i++) {
				if (ps[i].pid != -1 && ps[i].executiontime > 0){
					nextprocess = i;
					break;
				}
			}
		}
		else if ((current_time - lastcontextswitch) % 500 == 0)  {
			ps[runningprocess].readytime = current_time;
			qsort(ps, n, sizeof(struct process), cmp);

			nextprocess = (runningprocess + 1) % n;
			

			while (ps[nextprocess].pid == -1 || ps[nextprocess].executiontime == 0)
				nextprocess = (nextprocess + 1) % n;
		}
		else
			nextprocess = runningprocess;
	}

	//SJF
	else if (policy ==  2) {
		for (int i=0; i<n; i++) {
			if (ps[i].pid == -1 || ps[i].executiontime == 0) continue;
			if (nextprocess == -1) nextprocess = i;
			if (ps[i].executiontime < ps[nextprocess].executiontime) nextprocess = i;
		}
	}

	// PSJF
	else if (policy ==  3) {
		for (int i=0; i<n; i++) {
			if (ps[i].pid == -1 || ps[i].executiontime == 0) continue;
			if (nextprocess == -1) nextprocess = i;
			if (ps[i].executiontime < ps[nextprocess].executiontime) nextprocess = i;
		}
	}

	return nextprocess;
}

int scheduler(struct process *ps, int n, int policy){
	qsort(ps, n, sizeof(struct process), cmp);
	proc_assign_cpu(getpid(), PARENT_CPU); // Set single core prevent from preemption 
	wakeupprocess(getpid()); // Set high priority to scheduler
	
	current_time = 0;
	runningprocess = -1;
	num_finish_process = 0;
	
	while(1) {
		
		/* Check if runningprocess process finish */
		if (runningprocess != -1 && ps[runningprocess].executiontime == 0) {
			//kill(runningprocess, SIGKILL);
			waitpid(ps[runningprocess].pid, NULL, 0);
			printf("%s %d\n", ps[runningprocess].name, ps[runningprocess].pid);
			fflush(stdout);
			runningprocess = -1; num_finish_process++;
			if (num_finish_process == n) break;
		}

		// Check if process ready and execute 
		for (int i=0; i<n; i++){
			if (ps[i].readytime == current_time && ps[i].pid ==-1) {
				ps[i].pid = proc_exec(ps[i]);
				blockprocess(ps[i].pid);
			}

		}

		// Select next runningprocess  process 
		int next = next_process(ps, n, policy);
		if (next != -1) {
			// Context switch
			lastcontextswitch = current_time;
			if (runningprocess != next) {
				wakeupprocess(ps[next].pid);
				blockprocess(ps[runningprocess].pid);
				runningprocess = next;
				
			}
		}

		TIME_UNIT;

		if (runningprocess != -1) ps[runningprocess].executiontime--;
		current_time++;
	}

	return 0;
}
