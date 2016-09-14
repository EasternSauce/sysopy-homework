#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

int received;
int loop;
int wait;

void handler(int signal){
	if(signal == SIGUSR1){
		received++;
		wait = 0;
	}else if(signal == SIGUSR2){
		loop = 0;
	}
}

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Two arguments needed\n");
		return -1;
	}

	int signals = atoi(argv[1]);
	int pid = atoi(argv[2]);
	wait = 0;
	received = 0;
	loop = 1;

	signal(SIGUSR2, handler);
	signal(SIGUSR1, handler);

	for(int i = 0; i < signals; i++){
		kill(pid, SIGUSR1);
		wait = 1;
		while(wait);
	}

	kill(pid, SIGUSR2);

	while(loop);

	printf("Sent by sender: %d\n", signals);
	printf("Sent back to sender by receiver: %d\n", received);

	return 0;
}


