#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#define MAX 50
#define WRT 0
#define MUTEX 1

int *data;
sem_t *sems[2];
int interval = 1;
int readcount = 0;

void sem_down(int sem){
    if(sem_wait(sems[sem])==-1)
        printf("sem_wait error");
}
void sem_up(int sem){
    if(sem_post(sems[sem])==-1)
        printf("sem_post error");
}
int sem_count(int sem){
    int res;
    if(sem_getvalue(sems[sem],&res)==-1)
        printf("sem_getvalue error");
    return res;
}

void init_sem(){
    if((sems[MUTEX] = sem_open("mutex", O_CREAT | O_EXCL, 0644,1))==SEM_FAILED
	|| (sems[WRT] = sem_open("wrt", O_CREAT | O_EXCL, 0644,1))==SEM_FAILED){
            if(errno==EEXIST){
                if((sems[MUTEX] = sem_open("mutex",0,0644,0))==SEM_FAILED
                || (sems[WRT] = sem_open("wrt",0,0644,0))==SEM_FAILED)
                    printf("sem_open error");
            }
            else printf("sem_open error");
    }
}
void init_shm(){
    int fd;
    if((fd = shm_open("pc.c", O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR))!=-1){
        if (ftruncate(fd, sizeof(data)) == -1)
            printf("ftruncate error");
        data = mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED)
            printf("mmap error");
        for(int i=0; i<MAX;i++)data[i]=-1;
    }
    else if(errno==EEXIST){
        if((fd = shm_open("pc.c", O_RDWR, S_IRUSR | S_IWUSR))!=-1){
        if (ftruncate(fd, sizeof(data)) == -1)
            printf("ftruncate error");
        data = mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED)
            printf("mmap error");
        }
        else printf("shm_open error");
    }
    else printf("shm_open error");
}


void reader(){
	for(;;sleep(interval)){
		sem_down(MUTEX);
		readcount++;	
		if(readcount == 1) sem_down(WRT);
		sem_up(MUTEX);

		int searched_value = rand()%(10 + 1);
		int count = 0;

		for(int i = 0; i < MAX; i++) if(searched_value == data[i]) count ++;
		time_t raw_time;
		time(&raw_time);
		struct tm * time_info = localtime(&raw_time);

		printf("(%d %02d:%02d:%02d) znalazłem %d liczb o wartości %d\n", getpid(), time_info->tm_hour, time_info->tm_min, time_info->tm_sec, count, searched_value);

		sem_down(MUTEX);
		readcount--;
		if(readcount == 0) sem_up(WRT);
        sem_up(MUTEX);

	}
}
int main(int argc,char *argv[]){

    init_sem();
    init_shm();

    reader();
    return 0;
}
