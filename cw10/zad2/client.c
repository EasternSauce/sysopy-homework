//Socket chat client
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#define error(msg) {perror(msg); exit(1);}

#define PORT "4950"	// the port users will be connecting to
#define ever ;;

#define TXTSZ 256
#define NAMESZ 50

struct msg {
    char name[NAMESZ];
    char text[TXTSZ];
};

int sockfd;

char client_name[NAMESZ];

void *get_addr(struct sockaddr *sa) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

void* th_stdin(void* nvm) {
    int numbytes;
    struct msg buf;
    strcpy(buf.name,client_name);
    while(fgets(buf.text, TXTSZ, stdin) != NULL) {
        //strip newline
        char *pos;
        if ((pos=strchr(buf.text, '\n')) != NULL)*pos = '\0';
        numbytes = send(sockfd,&buf,sizeof(struct msg),0);
        if(numbytes <= 0)error("disconnected");
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    int rv;
    int numbytes;
    struct msg buf;
    char s[1000];

    if (argc != 4 && argc != 5) {
        printf("usage: %s clientname (0 path)|(1 hostname port)\n",argv[0]);
        return 0;
    }
    strcpy(client_name,argv[1]);
    if(!strcmp(argv[2],"0")) {
        //unix socket
        struct sockaddr_un remote;
        if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
            error("socket");
        remote.sun_family = AF_UNIX;
        strcpy(remote.sun_path, argv[3]);
        //connect
        if (connect(sockfd, (struct sockaddr *) &remote, sizeof(struct sockaddr_un)) == -1)
            error("connect");
    } else if(!strcmp(argv[2],"1")) {
        //internet socket
        struct addrinfo hints, *servinfo, *p;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if ((rv = getaddrinfo(argv[3],argv[4], &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }
        for(p = servinfo; p != NULL; p = p->ai_next) { //loop through suitable results and make a socket
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("socket");
                continue;
            }
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("connect");
                continue;
            }
            break;
        }
        if (p == NULL) {
            fprintf(stderr, "connection failed\n");
            return 2;
        }
        inet_ntop(p->ai_family, get_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
        printf("info: connected to %s\n", s);
        freeaddrinfo(servinfo);
    }
    //create thread for reading stdin
    pthread_t thread;
    if(pthread_create(&thread,NULL,th_stdin,NULL))error("pthread_create");
    //wait for replies on main thread
    for(ever) {
        if ((numbytes = recv(sockfd, &buf, sizeof(struct msg), 0)) <= 0) {
            printf("Disconnected\n");
            return 0;
        }
        printf("%s: %s\n",buf.name, buf.text);
    }
    close(sockfd);
    return 0;
}
