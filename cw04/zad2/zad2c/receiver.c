#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int received;
int loop;
pid_t pid;

void handler(int signal, siginfo_t *value, void* context){
	if(signal == SIGRTMIN+SIGUSR1){
		received++;
		pid = value->si_pid;
		printf("%d\n", received);
	}else if(signal == SIGRTMIN+SIGUSR2){
		loop = 0;
	}
}

int main(int argc, char* argv[]){
	received = 0;
	loop = 1;

	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = handler;
	sigaction(SIGRTMIN+SIGUSR1, &sig, NULL);
	sigaction(SIGRTMIN+SIGUSR2, &sig, NULL);

	while(loop);

	for(int i = 0; i < received; i++){
		kill(pid, SIGRTMIN+SIGUSR1);
	}

	kill(pid, SIGRTMIN+SIGUSR2);

	printf("Receiver received: %d\n", received);

	return 0;
}
