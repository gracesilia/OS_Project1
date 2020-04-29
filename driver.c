#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "process.h"
#include "schedpolicy.h"

int main(){

	char inp[10];
	int policy, n;
	scanf("%s",inp);
	scanf("%d", &n);

	struct process *ps;
	ps = (struct process *)malloc(n * sizeof(struct process));

	for (int i=0; i<n; i++){
		scanf("%s %d %d",ps[i].name, &ps[i].readytime, &ps[i].executiontime);
		ps[i].pid = -1; // imply not ready process
	}


	#ifdef DEBUG
	for (int i=0; i<n; i++){
		printf("%s %d %d\n",ps[i].name, ps[i].readytime, ps[i].executiontime);
		
	}
	#endif
	
	if (strcmp(inp, "FIFO") == 0) policy = 0;
	else if (strcmp(inp, "RR") == 0) policy = 1;
	else if (strcmp(inp, "SJF") == 0) policy = 2;
	else if (strcmp(inp, "PSJF") == 0) policy = 3;
	else {fprintf(stderr, "SCHEDULE POLICY: %s\n NOT FOUND", inp); exit(0);}

	scheduler(ps, n, policy);

	exit(0);
}
