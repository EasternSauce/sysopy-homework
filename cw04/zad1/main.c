#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

char * line = NULL;
int max;
int mul = 1;
int direction = 0;
int reverse = 1;

void main_handler(int signum){
	printf("\n");

	char * line_reversed = malloc(sizeof(char) * strlen(line));

	int j = 0;
	for(int i = strlen(line) - 1; i >= 0 ; i--, j++){
		line_reversed[j] = line[i];
	}

	strcpy(line, line_reversed);

	free(line_reversed);

	if(reverse == 1){
		reverse = 2;
	}
	else if(reverse == 2){
		reverse = 3;
	}
	else if(reverse == 3){
		if(direction == 0){
			if(mul == max) {direction = 1; mul--;}
			else mul++;
		}
		else if(direction == 1){
			if(mul == 1) {direction = 0; mul++;}
			else mul--;
		}
		reverse = 1;
	}
}

void exit_handler(int signum){
	printf("Odebrano sygnał SIGINT\n");
	
	exit(0);
}

int main(int argc, char * argv[]){
	signal(SIGTSTP, main_handler);

	struct sigaction sig;
	sig.sa_handler = exit_handler;
	sigaction(SIGINT, &sig, NULL);

	if(argc < 3){
		printf("Needs two arguments\n");
		return -1;
	}

	line = argv[1];
	max = atoi(argv[2]);

	while(1){
		for(int i = 0; i < mul; i++) printf("%s", argv[1]);
		printf("\n");
		sleep(1);
	}

	return 0;
}
