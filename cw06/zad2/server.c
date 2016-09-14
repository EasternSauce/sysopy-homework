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
mqd_t server_queue;
char client_names[100][32];
char buf[BUFSIZE];

void signalHandler(){
	mq_close(server_queue);
	exit(0);
}

int main(int argc, char * argv[]){
	srand(time(NULL));

	struct mq_attr attr;
	attr.mq_flags = O_RDWR;
	attr.mq_msgsize = BUFSIZE-10;
	attr.mq_curmsgs = 0;
	attr.mq_maxmsg = 10;

	signal(SIGQUIT, signalHandler);
	signal(SIGTSTP, signalHandler);
	signal(SIGINT, signalHandler);

	mqd_t server_queue = mq_open("/server", O_RDWR | O_CREAT | O_NONBLOCK, 0600, &attr);

	while(mq_receive(server_queue, buf, BUFSIZE, NULL) != -1);

	int client_id = 0;
	while(1){
		if(mq_receive(server_queue, buf, BUFSIZE, NULL) != -1){
			if(buf[0] == 1){
				char client_name[32];
				strcpy(client_name, buf+1);

				char id_to_send[32];
				sprintf(id_to_send, "%d", client_id);

				buf[0] = 1;
				strcpy(buf+1, id_to_send);

				mqd_t client_queue = mq_open(client_name, O_RDWR | O_NONBLOCK);

				strcpy(client_names[client_id], client_name);

				if(mq_send(client_queue, buf, strlen(buf) + 1, 0)){
					printf("ID cannot be sent: %s", strerror(errno));
					return -1;
				}

				client_id++;
				mq_close(client_queue);
			}
			else if(buf[0] == 2){
				int client_id = atoi(buf+1);
				char number[32];
				int random = rand();
				sprintf(number, "%d", random);
				strcpy(buf+1, number);
				buf[0] = 2;

				mqd_t client_queue = mq_open(client_names[client_id], O_WRONLY | O_NONBLOCK);

				if(mq_send(client_queue, buf, strlen(buf)+1, 0)){
					printf("number cannot be sent: %s\n", strerror(errno));
					printf("length of msg: %d\n", (int)strlen(buf));
					return -1;
				}
				mq_close(client_queue);
			}
			else if(buf[0] == 3){
				int client_id, number, is_num_prime;
				sscanf(buf+1, "%d,%d,%d", &client_id, &number, &is_num_prime);

				if(is_num_prime){
					printf("Liczba pierwsza: %d (klient: %d)\n", number, client_id);
					fflush(stdout);
				}
				buf[0] = 3;

				mqd_t client_queue = mq_open(client_names[client_id], O_WRONLY | O_NONBLOCK);

				mq_send(client_queue, buf, strlen(buf)+1, 0);

				mq_close(client_queue);
			}
		}
		memset(buf, 0, BUFSIZE);
	}
	return 0;
}
