#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define TEXTSIZE 256
#define NAMESIZE 50
#define EXPIRATION 100
#define MAXCLIENTS 3

struct msg {
	char name[NAMESIZE];
	char text[TEXTSIZE];
};

int unixfd;
int inetfd;

char client_names[MAXCLIENTS][NAMESIZE];
struct sockaddr client_addr[MAXCLIENTS];
time_t client_regtime[MAXCLIENTS];
int client_sockfd[MAXCLIENTS];

char path[256];
char port[10];

void *get_in_addr(struct sockaddr *sa) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int get_socket(int family) {
	int sockfd;

	if(family==AF_INET) {
		struct addrinfo hints, *servinfo, *p;
		servinfo = NULL;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = family;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;


		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
				continue;
			}
			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				continue;
			}
			break;
		}
		if (p == NULL) {
			fprintf(stderr, "failed to bind socket\n");
			exit(1);
		}
		freeaddrinfo(servinfo);

	} else if(family==AF_UNIX) {
		sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

		struct sockaddr_un local;
		local.sun_family = AF_UNIX;
		strcpy(local.sun_path, path);
		unlink(local.sun_path);
		bind(sockfd, (struct sockaddr *)&local, strlen(local.sun_path) + sizeof(local.sun_family));
	}
	return sockfd;
}


void handle(int sockfd) {
	struct sockaddr their_addr;
	struct msg buf;

	printf("info: got packet from %s\n", buf.name);
	int i=0;
	for(; i<MAXCLIENTS; i++)
		if(client_regtime[i]!=-1  && strcmp(buf.name,client_names[i])==0  && client_regtime[i]-time(NULL)>-EXPIRATION)break;

	if(strcmp(buf.text,"reg")==0) {
		if(i<MAXCLIENTS) {
			client_regtime[i]=time(NULL);
			printf("info: client renewed\n");
			strcpy(buf.text,"Renewed.");
		} else {
			for(i=0; i<MAXCLIENTS && client_regtime[i]-time(NULL)>-EXPIRATION; i++);
			if(i!=MAXCLIENTS) {
				strcpy(client_names[i],buf.name);
				client_addr[i]=their_addr;
				client_regtime[i]=time(NULL);
				client_sockfd[i] = sockfd;
				strcpy(buf.text,"Registered.");
				printf("info: client registered\n");
			} else {
				strcpy(buf.text,"No client slots available.");
				printf("info: no slot for client\n");
			}
		}
	} else {
		if(i<MAXCLIENTS) {
			printf("broadcasting msg\n");
			for(int j = 0; j<MAXCLIENTS; j++)
				if(j!=i && client_regtime[j]-time(NULL)>-EXPIRATION)
					strcpy(buf.text,"ok");
		} else {
			strcpy(buf.text,"send reg to register");
		}
	}
	strcpy(buf.name,"Server");
}

void quit(int unused) {
	close(inetfd);
	close(unixfd);
	exit(0);
}

int main(int argc, char* argv[]) {
	struct timeval mtv,tv;
	fd_set readfds,master;
	mtv.tv_sec = 2;
	mtv.tv_usec = 0;

	if(argc!=3) {
		printf("arguments: %s port path\n",argv[0]);
		return 0;
	}

	strcpy(port,argv[1]);
	strcpy(path,argv[2]);
	signal(SIGINT,quit);

	for(int i=0; i<MAXCLIENTS; i++)client_regtime[i] = -1;

	inetfd = get_socket(AF_INET);
	unixfd = get_socket(AF_UNIX);
	FD_ZERO(&master);
	FD_SET(inetfd, &master);
	FD_SET(unixfd, &master);
	int bigger = inetfd > unixfd ? inetfd : unixfd;
	printf("waiting to recvfrom\n");

	while(1){
		readfds = master;
		tv = mtv;
		select(bigger+1, &readfds, NULL, NULL, &tv);
		if (FD_ISSET(inetfd, &readfds)) handle(inetfd);
		if (FD_ISSET(unixfd, &readfds)) handle(unixfd);
		printf("Waiting\n");
	}
}
