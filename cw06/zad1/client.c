#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

int client_queue;

struct buffer{
	long type;
	char value[32];
};

int is_prime(int number){
	int flag = 0;
	for(int i = 2; i <= number/2; i++){
		if(number % i == 0){
			flag = 1;
			break;
		}
	}

	if(flag != 0) return 0;
	else return 1;
}

void signalHandler(){
	msgctl(client_queue, IPC_RMID, NULL);
	exit(0);
}

int main(int argc, char * argv[]){
	if(argc != 3){
		printf("Needs 2 arguments\n");
		return -1;
	}

	signal(SIGQUIT, signalHandler);
	signal(SIGTSTP, signalHandler);
	signal(SIGINT, signalHandler);

	key_t queue_key = ftok(argv[1], argv[2][0]);

	struct buffer buf;

	int server_queue = msgget(queue_key, 0600);

	client_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0600);

	char id_to_send[32];
	int client_id;

	sprintf(id_to_send, "%d", client_queue);

	buf.type = 1;
	strcpy(buf.value, id_to_send);

	if(msgsnd(server_queue, &buf, sizeof buf, 0) == -1){
		printf("Problem sending a message\n");
		return -1;
	}

	while(1){
		if(msgrcv(client_queue, &buf, sizeof buf, 0, IPC_NOWAIT) != -1){
			if(buf.type == 1){
				client_id = atoi(buf.value);
				buf.type = 2;

				msgsnd(server_queue, &buf, sizeof buf, 0);


			}
			else if(buf.type == 2){
				int number = atoi(buf.value);
				int is_num_prime = is_prime(number);

				char msg[32];
				sprintf(msg, "%d %d %d", client_id, number, is_num_prime);

				strcpy(buf.value, msg);
				buf.type = 3;

				msgsnd(server_queue, &buf, sizeof buf, 0);

			}
			else if(buf.type == 3){
				char id_to_send[32]; 
				sprintf(id_to_send, "%d", client_id);
				strcpy(buf.value, id_to_send);
				buf.type = 2;
				msgsnd(server_queue, &buf, sizeof buf, 0);
			}

		}
	}

	return 0;
}
