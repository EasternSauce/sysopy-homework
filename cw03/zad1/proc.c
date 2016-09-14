#define _GNU_SOURCE 
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

long counter;
long parent_time;

int fn(){
	counter++;
	long t = times(NULL);
	_exit(t - parent_time);
}

int main(int argc, char ** argv){
	if(argc != 3){
		printf("needs two arguments\n");
		return -1;
	}

	char method[16];
	strcpy(method, argv[1]);
	long n = atoi(argv[2]);
	long child_time = 0;
	long start_time = times(NULL);
	void * stack = malloc(8192);
	counter = 0;

	for(int i = 0; i < n; i++){
		parent_time = times(NULL);

		pid_t pid;
		if(strcmp(method, "fork") == 0){
			pid = fork();
		}
		else if(strcmp(method, "vfork") == 0){
			pid = vfork();
		}
		else if(strcmp(method, "clonefork") == 0){
			pid = clone(fn, stack + 8192, SIGCHLD, NULL);
		}
		else if(strcmp(method, "clonevfork") == 0){
			pid = clone(fn, stack + 8192, SIGCHLD | CLONE_VM | CLONE_VFORK | CLONE_FS | CLONE_FILES, NULL);
		}
		else{
			printf("unknown method\n");
			return -1;
		}

		if(pid < 0){
			fprintf(stderr, "pid error\n");
		}
		else if(pid == 0){
			fn();
		}
		else{
			int status;
			wait(&status);
			child_time += WEXITSTATUS(status);
		}

	}

	printf("counter =  %ld\n", counter);

	struct tms tms_struct;
	long t = times(&tms_struct);
	parent_time = t - start_time;

	float clk = (float)sysconf(_SC_CLK_TCK);

	printf("parent\n");
	printf("real: %f, user: %f, sys: %f\n", parent_time / clk, tms_struct.tms_utime / clk, tms_struct.tms_stime / clk);

	printf("children\n");
	printf("real: %f, user: %f, sys: %f\n", child_time / clk, tms_struct.tms_cutime / clk, tms_struct.tms_cstime / clk);

	return 0;
}
