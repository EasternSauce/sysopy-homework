#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RECORD_SIZE 1024

pthread_t *threads;
pthread_mutex_t mutex;
int threads_count, records_count, fd;
char* word;

void* thread_func(void *data){
	pthread_t self;
	int read_count,i,j,record_id,actual_read_count,word_len;
	char *buf,*ptr;

	buf=malloc(RECORD_SIZE*records_count);


	word_len=strlen(word);
	self=pthread_self();

	read_count=1;
	while(read_count){

		pthread_mutex_lock(&mutex);
		read_count=read(fd,buf,RECORD_SIZE*records_count);
		pthread_mutex_unlock(&mutex);

		actual_read_count=read_count/RECORD_SIZE;
		ptr=buf;
		for(i=0;i<actual_read_count;i++){
			record_id=atoi(ptr);

			for(j=5;j<RECORD_SIZE-word_len;j++){
				if(word_eq(ptr+j,word_len)){
					printf("Znaleziono, TID - %d, rekord - %d\n",(unsigned int)self,record_id);
				}
			}

			ptr+=RECORD_SIZE;
		}

	}

	return NULL;
}

bool word_eq(char* s, int word_len){
	int i;
	char *w =word;
	if(!(s-1) || *(s-1)==' '){

		for(i=0; i<word_len; i++){
			if(s){
				if(*w != *s){
					return false;
				}
			}
			w++;
			s++;
		}

		if(!(s) || *(s)==' ')
			return true;
	}
	return false;
}

void handler(int unused){
	exit(0);
}

int main(int argc, char **argv){
	char *file_name;
	int i;

	if(argc < 5){
		printf("Needs four arguments\n");
		return -1;
	}else{
		threads_count=atoi(argv[1]);
		file_name=argv[2];
		records_count=atoi(argv[3]);
		word=argv[4];
	}


	threads=malloc(sizeof(pthread_t)*threads_count);

	pthread_mutex_init(&mutex,NULL);
	fd=open(file_name,O_RDONLY);
	signal(SIGSEGV,handler);

	for(i=0; i < threads_count; i++){
		pthread_create(threads+i,NULL,&thread_func,NULL);
	}

	for(i=0; i < threads_count; i++){
		pthread_join(threads[i],NULL);
	}

	close(fd);
	pthread_mutex_destroy(&mutex);
	free(threads);
	return 0;
}

