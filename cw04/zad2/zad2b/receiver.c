#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int received;
int loop;
pid_t pid;

void handler(int signal, siginfo_t *value, void* context){
	if(signal == SIGUSR1){
		received++;
		pid = value->si_pid;
		printf("%d\n", received);
		kill(pid, SIGUSR1);
	}else if(signal == SIGUSR2){
		loop = 0;
	}
}

int main(int argc, char* argv[]){
	received = 0;
	loop = 1;

	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = handler;
	sigaction(SIGUSR1, &sig, NULL);
	sigaction(SIGUSR2, &sig, NULL);

	while(loop);

	kill(pid, SIGUSR2);

	printf("Receiver received: %d\n", received);

	return 0;
}
