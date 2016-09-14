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

void writer(){
    for(;;sleep(interval)){
		int how_many = rand()%MAX/2;
		int positions[MAX];
		for(int i = 0; i < MAX; i++) positions[i] = -1;
		while(how_many > 0){
			int position = rand()%MAX;
			if(positions[position] == -1){
				positions[position] = 1;
				how_many--;
			}
		}

		sem_down(WRT);
		for(int i = 0; i < MAX; i++){
			if(positions[i] == 1){
				if(data[i] == -1){
					data[i] = rand()%10;
				}
				else if(data[i] < 10){
					data[i]++;
				}

				time_t raw_time;
				time(&raw_time);
				struct tm * time_info = localtime(&raw_time);

				printf("(%d %02d:%02d:%02d) Wpisałem liczbę %d - na pozycję %d\n", getpid(),time_info->tm_hour, time_info->tm_min, time_info->tm_sec, data[i], i);
			}
		}
		
        sem_up(WRT);
    }
}


int main(int argc,char *argv[]){

    init_sem();
    init_shm();

    srand(time(NULL));
    writer();
    return 0;
}
