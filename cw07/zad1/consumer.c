#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#define MAX 50
#define MUTEX 2
#define FILL 1
#define EMPTY 0

int interval = 1;
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int semid;
void change_sem(int sem,int val){
	struct sembuf sb;
	sb.sem_num = sem;
	sb.sem_op = val;
	sb.sem_flg = 0;
	if (semop(semid, &sb, 1) == -1) printf("semop error");
}


void init_sem(){
	key_t key = ftok("producer.c", 'A');
	union semun arg;
	struct semid_ds buf;
	if((semid = semget(key, 3, 0644 | IPC_CREAT | IPC_EXCL))==-1){
		if(errno == EEXIST){
			if((semid = semget(key, 3, 0)) == -1) printf("semget error");
			arg.buf = &buf;
			for(arg.buf->sem_otime=0;arg.buf->sem_otime==0;sleep(1))
				if(semctl(semid, 0, IPC_STAT, arg)==-1)
					printf("semctl error");
		}
		else printf("semget error");
	}
	else {
		change_sem(MUTEX,1);
		change_sem(EMPTY,MAX);
	}
}


int sem_count(int sem){
	int res;
	if((res = semctl(semid, sem, GETVAL))==-1)
		printf("semctl error");
	return res;
}


void sem_down(int sem){
	change_sem(sem,-1);
}

void sem_up(int sem){
	change_sem(sem,1);
}

int *data;
void init_shm(){
	key_t key;
	int shmid;
	if((key = ftok("producer.c", 'B')) == -1)
		printf("ftok error");

	if((shmid = shmget(key, MAX*sizeof(int), 0644 | IPC_CREAT | IPC_EXCL)) == -1) {
		if(errno == EEXIST){
			if((shmid = shmget(key, MAX*sizeof(int), 0644)) == -1)
				printf("shmget error");
			data = shmat(shmid, (void *)0, 0);
			if (data == (int *)(-1))
				printf("shmat error");
		}
	}
	else {
		data = shmat(shmid, (void *)0, 0);
		if ((char*)data == (char *)(-1))
			printf("shmat error");
		for(int i=0; i<MAX;i++)data[i]=-1;
	}
}


void consumer(){
	for(;;sleep(interval)){
		sem_down(FILL);
		sem_down(MUTEX);
		int i = 0;
		for(;data[i]==-1;i++);
		int to_check = data[i];
		data[i]=-1;
		sem_up(MUTEX);
		sem_up(EMPTY);

		time_t raw_time;
		time(&raw_time);
		struct tm * time_info = localtime(&raw_time);

		printf("(%d %02d:%02d:%02d) Sprawdziłem liczbę %d na pozycji %d - %s. Pozostało zadań oczekujących: %d\n",getpid(), time_info->tm_hour, time_info->tm_min, time_info->tm_sec, to_check,i,(to_check%2==0 ? "parzysta" : "nieparzysta"),sem_count(FILL));
	}
}


int main(int argc,char *argv[]){

    init_sem();
    init_shm();

	consumer();
    return 0;
}
