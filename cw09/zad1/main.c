#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define N 5
#define THINK 0
#define WAIT 1
#define EAT 2

pthread_cond_t condition[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int philo_state[N];
pthread_t threads[N];

void change_philo_state(int i) {
    if (philo_state[i] == WAIT && philo_state[(i+N-1)%N] != EAT && philo_state[(i+1)%N] != EAT) {
        philo_state[i] = EAT;
        pthread_cond_signal(&condition[i]);
    }
}

void cleanup(void* args){
    pthread_mutex_unlock(&mutex);
}

void use(int i) {
    pthread_mutex_lock(&mutex);
    pthread_cleanup_push(cleanup,NULL);
    philo_state[i] = WAIT;
    change_philo_state(i);
    while(philo_state[i] == WAIT)
        pthread_cond_wait(&condition[i],&mutex);
    pthread_cleanup_pop(1);
}

void put_back(int i) {
    pthread_mutex_lock(&mutex);
    pthread_cleanup_push(cleanup,NULL);
    philo_state[i] = THINK;
    change_philo_state((i+N-1)%N);
    change_philo_state((i+1)%N);
    pthread_cleanup_pop(1);
}

void* philo(void *arg) {
    int id = *(int*)arg;
    while(1) {
        usleep(rand()%100000);
        use(id);
        printf("philosopher id %d is eating\n",id);
        usleep(rand()%100000);
        put_back(id);
    }
}

void handler(int unused) {
    for(int i=0; i<N; i++)
        pthread_cancel(threads[i]);
}

int main() {
	for (int i = 0; i < N; i++) {
		pthread_cond_init(&condition[i], NULL);
		philo_state[i] = THINK;
	}

    srand(time(NULL));

    for(int i = 0; i < N; i++) {
        int *id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, philo, id);
    }
    signal(SIGINT, handler);

    for(int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
