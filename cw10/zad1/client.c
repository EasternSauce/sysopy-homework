#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT "5123"
#define TEXTSIZE 256
#define NAMESIZE 50

struct msg{
	char name[NAMESIZE];
	char text[TEXTSIZE];
};

int sockfd;
struct sockaddr* addr;
char client_name[NAMESIZE];
socklen_t sz;

void* mythread(void* unused) {
	struct msg buf;
	strcpy(buf.name,client_name);

	while(fgets(buf.text, TEXTSIZE, stdin) != NULL) {

		char *pos;
		if ((pos=strchr(buf.text, '\n')) != NULL)*pos = '\0';

	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int rv;
	struct msg buf;

	if (argc != 4 && argc != 5) {
		printf("arguments: %s clientname (0 path) or (1 hostname port)\n",argv[0]);
		return 0;
	}

	strcpy(client_name,argv[1]);

	if(!strcmp(argv[2],"0")) {
		struct sockaddr_un remote, claddr;
		sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
		remote.sun_family = AF_UNIX;
		strcpy(remote.sun_path, argv[3]);
		addr = (struct sockaddr*)&remote;
		sz = sizeof(struct sockaddr_un);
		memset(&claddr, 0, sizeof(struct sockaddr_un));
		claddr.sun_family = AF_UNIX;
		snprintf(claddr.sun_path, sizeof(claddr.sun_path),"/tmp/so.%ld", (long) getpid());
		bind(sockfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un));
	}
	else if(!strcmp(argv[2],"1")) {
		struct addrinfo hints, *servinfo, *p;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		if ((rv = getaddrinfo(argv[3],argv[4], &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return 1;
		}

		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
				continue;
			}
			break;
		}
		if (p == NULL) {
			fprintf(stderr, "failed to get socket\n");
			return 2;
		}
		addr = p->ai_addr;
		sz = p->ai_addrlen;
		freeaddrinfo(servinfo);
	}

	pthread_t thread;
	pthread_create(&thread,NULL,mythread,NULL);
	while(1) {
		if(strcmp(buf.text,"ok"))printf("%s: %s\n",buf.name, buf.text);
	}
	close(sockfd);


	return 0;
}
