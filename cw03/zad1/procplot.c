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

#define N_ITER 50000

long counter;
long parent_time;

int fn(){
	counter++;
	long t = times(NULL);
	_exit(t - parent_time);
}

int main(int argc, char ** argv){

	float real_times[3][4][4]; //fork, vfork, cfork, cvfork
	float user_times[3][4][4];
	float sys_times[3][4][4];

	char method[16];
	for(int m = 0; m < 4; m++){
		switch(m){
			case 0:
				strcpy(method, "fork");
				break;
			case 1:
				strcpy(method, "vfork");
				break;
			case 2:
				strcpy(method, "clonefork");
				break;
			case 3:
				strcpy(method, "clonevfork");
				break;
		}


		for(int i = 0; i < 4; i++){

			long n = (i+1) * N_ITER;
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

			//printf("counter =  %ld\n", counter);

			struct tms tms_struct;
			long t = times(&tms_struct);
			parent_time = t - start_time;

			float clk = (float)sysconf(_SC_CLK_TCK);

			//printf("parent\n");
			//printf("%d,%f,%f,%f\n", n, parent_time / clk, tms_struct.tms_utime / clk, tms_struct.tms_stime / clk);
			real_times[0][i][m] = parent_time / clk;
			user_times[0][i][m] = tms_struct.tms_utime / clk;
			sys_times[0][i][m] = tms_struct.tms_stime / clk;

			//printf("children\n");
			//printf("%d,%f,%f,%f\n", n, child_time / clk, tms_struct.tms_cutime / clk, tms_struct.tms_cstime / clk);
			real_times[1][i][m] = parent_time / clk;
			user_times[1][i][m] = tms_struct.tms_utime / clk;
			sys_times[1][i][m] = tms_struct.tms_stime / clk;

			real_times[2][i][m] = real_times[0][i][m] + real_times[1][i][m];
			user_times[2][i][m] = user_times[0][i][m] + user_times[1][i][m];
			sys_times[2][i][m] = sys_times[0][i][m] + sys_times[1][i][m];

		}

	}

	//for every real/user/sys/user+sys
	for(int i = 0; i < 4; i++){
		
		//for parent/child/parent+child
		for(int j = 0; j < 3; j++){
			char filename[] = "";
			switch(i){
				case 0:
					strcat(filename, "real");
					break;
				case 1:
					strcat(filename, "user");
					break;
				case 2:
					strcat(filename, "sys");
					break;
				case 3:
					strcat(filename, "user&sys");
					break;
			}
			switch(j){
				case 0:
					strcat(filename, "_parent");
					break;
				case 1:
					strcat(filename, "_child");
					break;
				case 2:
					strcat(filename, "_parent&child");
					break;
			}
			strcat(filename, ".txt");

			FILE * f = fopen(filename, "w");
			
			fprintf(f, "n,fork,vfork,clonefork,clonevfork\n");
			//for every n
			for(int k = 0; k < 4; k++){
				int n = (k+1) * N_ITER;
				fprintf(f, "%d", n);
				for(int p = 0; p < 4; p++){
					switch(i){
						case 0:
							fprintf(f, ",%f", n, real_times[j][k][p]);
							break;
						case 1:
							fprintf(f, ",%f", n, user_times[j][k][p]);
							break;
						case 2:
							fprintf(f, ",%f", n, sys_times[j][k][p]);
							break;
						case 3:
							fprintf(f, ",%f", n, user_times[j][k][p] + sys_times[j][k][p]);
							break;

					}
				}

				fprintf(f, "\n");
			}
			fclose(f);
		}
	}

	return 0;
}
