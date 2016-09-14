#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define N 10
#define K 5
#define NUM_OF_PLANES 15

int planes_on_board;
int waiting_to_takeoff;
int waiting_to_land;

pthread_cond_t took_off,landed;
pthread_t planes[NUM_OF_PLANES];
int plane_id[NUM_OF_PLANES];

pthread_mutex_t landingstrip;
pthread_mutex_t waiting_to_takeoff_mutex;
pthread_mutex_t waiting_to_land_mutex;


void* plane(void* plane_id){
	int id = *((int*) plane_id);
	printf("Plane ID %d, waiting to land\n", id);

	while(1){

		printf("Plane ID %d is in the air, planes on board: %d\n", id,planes_on_board);
		usleep(500000);

		pthread_mutex_lock(&waiting_to_land_mutex);
		waiting_to_land++;

		pthread_mutex_unlock(&waiting_to_land_mutex);
		pthread_mutex_lock(&landingstrip);

		pthread_mutex_lock(&waiting_to_takeoff_mutex);

		while(planes_on_board==N || (planes_on_board >= K && waiting_to_takeoff > 0)){
			pthread_mutex_unlock(&waiting_to_takeoff_mutex);
			pthread_cond_wait(&took_off, &landingstrip);
			pthread_mutex_lock(&waiting_to_takeoff_mutex);
		}

		pthread_mutex_unlock(&waiting_to_takeoff_mutex);

		pthread_mutex_lock(&waiting_to_land_mutex);
		waiting_to_land--;
		pthread_mutex_unlock(&waiting_to_land_mutex);

		planes_on_board++;
		pthread_cond_signal(&landed);
		pthread_mutex_unlock(&landingstrip);

		printf("Plane ID %d landed, number of planes on board: %d\n",id,planes_on_board);
		usleep(500000);

		pthread_mutex_lock(&waiting_to_takeoff_mutex);
		waiting_to_takeoff++;
		pthread_mutex_unlock(&waiting_to_takeoff_mutex);
		pthread_mutex_lock(&landingstrip);

		pthread_mutex_lock(&waiting_to_land_mutex);
		while(planes_on_board<K && waiting_to_land>0){
			pthread_mutex_unlock(&waiting_to_land_mutex);
			pthread_cond_wait(&landed,&landingstrip);
			pthread_mutex_lock(&waiting_to_land_mutex);
		} 
		pthread_mutex_unlock(&waiting_to_land_mutex);

		pthread_mutex_lock(&waiting_to_takeoff_mutex);
		waiting_to_takeoff--;
		pthread_mutex_unlock(&waiting_to_takeoff_mutex);

		planes_on_board--;
		pthread_cond_signal(&took_off);
		pthread_mutex_unlock(&landingstrip);
	}

	return NULL;
}

int main(){
	int i;
	pthread_mutexattr_t attr;

	planes_on_board = 0;
	waiting_to_takeoff = 0;
	waiting_to_land = 0;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);

	pthread_mutex_init(&landingstrip,&attr);
	pthread_mutex_init(&waiting_to_land_mutex,&attr);
	pthread_mutex_init(&waiting_to_takeoff_mutex,&attr);

	pthread_cond_init(&took_off, NULL);
	pthread_cond_init(&landed, NULL);

	for(i=0;i<NUM_OF_PLANES;i++){
		plane_id[i]=i;
	}

	for (i = 0; i < NUM_OF_PLANES; i++) {
		pthread_create(planes+i, NULL, plane, plane_id+i);
	}
	for (i = 0; i < NUM_OF_PLANES; i++) {
		pthread_join(planes[i], NULL);
	}

	pthread_mutex_destroy(&landingstrip);
	pthread_mutex_destroy(&waiting_to_takeoff_mutex);
	pthread_mutex_destroy(&waiting_to_land_mutex);
	pthread_cond_destroy(&took_off);
	pthread_cond_destroy(&landed);

	return 0;
}


