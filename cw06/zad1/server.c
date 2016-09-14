#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

int server_queue;

struct buffer{
	long type;
	char value[32];
};

void signalHandler(){
	msgctl(server_queue, IPC_RMID, NULL);
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

	server_queue = msgget(queue_key, IPC_CREAT | IPC_EXCL | 0600);
	if(server_queue == -1) return -1;
	while(msgrcv(server_queue, &buf, sizeof buf, 0, IPC_NOWAIT) != -1);

	int client_queues[100];

	int client_id = 0;
	while(1){
		if(msgrcv(server_queue, &buf, sizeof buf, 0, IPC_NOWAIT) != -1){
			if(buf.type == 1){
				int client_queue = atoi(buf.value); 
				client_queues[client_id] = client_queue;

				char id_to_send[32];	
				sprintf(id_to_send, "%d", client_id++);
				buf.type = 1;
				strcpy(buf.value, id_to_send);

				if(msgsnd(client_queue, &buf, sizeof buf, 0) == -1){
					printf("ID cannot be sent");
					return -1;
				}
			}
			else if(buf.type == 2){
				int client_id = atoi(buf.value);
				buf.type = 2;
				char number[32];
				sprintf(number, "%d", rand());
				strcpy(buf.value, number);
				msgsnd(client_queues[client_id], &buf, sizeof buf, 0);

			}
			else if(buf.type == 3){
				int client_id, number, is_num_prime;
				sscanf(buf.value, "%d %d %d", &client_id, &number, &is_num_prime);

				if(is_num_prime){
					printf("Liczba pierwsza: %d (klient: %d)\n", number, client_id);
					fflush(stdout);
				}
				buf.type=3;
				msgsnd(client_queues[client_id], &buf, sizeof buf, 0);

			}
		}
	}

	return 0;
}
