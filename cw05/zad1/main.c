#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[]){
	if(argc != 2){
		printf("Needs one argument");
		return -1;
	}

	char * n = argv[1];

	int fd1[2];
	pid_t pid1;
	pipe(fd1);
	pid1 = fork();
	
	if(pid1 > 0){
		close(fd1[0]);
		dup2(fd1[1], 1);
		execl("/bin/tr", "tr", "[:lower:]", "[:upper:]", NULL);
	}

	else if(pid1 == 0){
		close(fd1[1]);
		dup2(fd1[0], 0);

		int fd2[2];
		pipe(fd2);
		execl("/bin/fold", "fold", "-w", n, NULL);
	}

	return 0;
}

