//Socket chat server. Dawid Dworak 2015
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
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
#define ever ;;
#define error(msg) {perror(msg); exit(1);}

#define TXTSZ 256
#define NAMESZ 50
#define MAXCLIENTS 3
#define EXPIRATION 100

struct msg {
    char name[NAMESZ];
    char text[TXTSZ];
};

int unixfd; //UNIX datagram socket
int inetfd; //INET datagram socket
int biggest;
fd_set readfds,master;
char path[256];
char port[10];

//open socket
int get_socket(int family) {
    int sockfd;
    int rv;
    if(family==AF_INET) {
        struct addrinfo hints, *servinfo, *p;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = family; // AF_INET -> force IPv4, AF_UNSPEC -> v4/v6
        hints.ai_socktype = SOCK_STREAM; //stream
        hints.ai_flags = AI_PASSIVE; // use any of host's network addresses

        if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) //get possible socket addresses
            error("getaddrinfo");

        for(p = servinfo; p != NULL; p = p->ai_next) {  //bind to first available
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("socket");
                continue;
            }
            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("bind");
                continue;
            }
            break;
        }
        if (p == NULL) {
            fprintf(stderr, "error: failed to bind socket\n");
            exit(1);
        }
        freeaddrinfo(servinfo);
    } else if(family==AF_UNIX) {
        if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
            error("socket");
        struct sockaddr_un local;
        local.sun_family = AF_UNIX;
        strcpy(local.sun_path, path);
        unlink(local.sun_path);
        if(bind(sockfd, (struct sockaddr *)&local, strlen(local.sun_path) + sizeof(local.sun_family)))error("bind");
    }
    //all good
    if(listen(sockfd,5))error("listen");
    return sockfd;
}
void handlereg(int sockfd) {
    //someone wants to connect!
    struct sockaddr remoteaddr;
    socklen_t addrlen = sizeof remoteaddr;
    int newfd = accept(sockfd,&remoteaddr,&addrlen);
    if(newfd==-1)error("accept");
    FD_SET(newfd,&master); //add to "selected"
    if(newfd > biggest)biggest = newfd;
    printf("info: new connection\n");
}

void handlemsg(int sockfd) { //sth waiting
    struct msg buf;
    int numbytes;
    if ((numbytes = recv(sockfd,&buf,sizeof(struct msg),0)) == -1)
        error("recv");
    if(numbytes <= 0) {
        //disconnected
        printf("info: client disconnected\n");
        close(sockfd);
        FD_CLR(sockfd,&master);
    } else {
        //received client msg
        printf("info: got packet from %s\n", buf.name);
        printf("info: broadcasting msg\n");
        for(int j = 0; j<=biggest; j++) //broadcast to everyone else
            if(j!=sockfd && j!=unixfd && j!=inetfd && FD_ISSET(j,&master))
                if ((numbytes = send(j,&buf,sizeof(struct msg),0)) <= 0){
                    //disconnected
                    printf("info: client disconnected\n");
                    close(j);
                    FD_CLR(j,&master);
                }
    }
}
void quit(int nvm) {
    for(int i=0; i<=biggest; i++)
        if(FD_ISSET(i,&master))close(i);
    exit(0);
}
int main(int argc, char* argv[]) {
    struct timeval mtv,tv;
    mtv.tv_sec = 2;
    mtv.tv_usec = 0;
    if(argc!=3) {
        printf("usage: %s port path\n",argv[0]);
        return 0;
    }
    strcpy(port,argv[1]);
    strcpy(path,argv[2]);
    signal(SIGINT,quit);

    inetfd = get_socket(AF_INET); //get inet socket
    unixfd = get_socket(AF_UNIX); //get unix socket
    FD_ZERO(&master);
    FD_SET(inetfd, &master);
    FD_SET(unixfd, &master);
    biggest = inetfd > unixfd ? inetfd : unixfd;
    printf("info: waiting to recvfrom...\n");
    for(ever) {
        readfds = master;
        tv = mtv;
        select(biggest+1, &readfds, NULL, NULL, &tv);
        for(int i=0; i<= biggest; i++)
            if(FD_ISSET(i,&readfds)) {
                if(i == inetfd || i==unixfd)handlereg(i);
                else handlemsg(i);
            }
        printf("Waiting...\n");
    }
}
