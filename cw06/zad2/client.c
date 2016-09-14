#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define BUFSIZE 1024

mqd_t client_queue, server_queue;
char client_name[32];
char buf[BUFSIZE];
int client_id;

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
	mq_unlink(client_name);
	exit(0);
}

int main(int argc, char * argv[]){
	srand(time(NULL));

	sprintf(client_name, "/%d", rand());

	struct mq_attr attr;
	attr.mq_flags = O_RDWR;
	attr.mq_msgsize = BUFSIZE-10;
	attr.mq_curmsgs = 0;
	attr.mq_maxmsg = 10;

	signal(SIGQUIT, signalHandler);
	signal(SIGTSTP, signalHandler);
	signal(SIGINT, signalHandler);

	server_queue = mq_open("/server", O_RDWR | O_NONBLOCK);

	client_queue = mq_open(client_name, O_RDWR | O_CREAT | O_NONBLOCK, 0600, &attr);


	buf[0] = 1;
	strcpy((buf+1), client_name);

	if(mq_send(server_queue, buf, strlen(buf), 0) == -1){
		printf("Problem sending a message: %s\n", strerror(errno));
		return -1;
	}

	while(1){
		if(mq_receive(client_queue, buf, BUFSIZE, NULL) != -1){
			if(buf[0] == 1){
				client_id = atoi(buf+1);
				buf[0] = 2;

				mq_send(server_queue, buf, strlen(buf), 0);

			}
			else if(buf[0] == 2){

				int number = atoi(buf+1);

				int is_num_prime = is_prime(number);

				char msg[32];
				snprintf(msg, 32, "%d,%d,%d", client_id, number, is_num_prime);
				strcpy(buf+1, msg);
				buf[0]= 3;

				if(mq_send(server_queue, buf, strlen(buf), 0) == -1){
					printf("Error sending data: %s\n", strerror(errno));
				}

			}
			else if(buf[0] == 3){
				char id_to_send[32]; 
				sprintf(id_to_send, "%d", client_id);
				strcpy(buf+1, id_to_send);
				buf[0] = 2;
				mq_send(server_queue, buf, strlen(buf), 0);


			}

		}
		memset(buf, 0, BUFSIZE);
	}

	return 0;
}
