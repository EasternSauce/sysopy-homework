#define _GNU_SOURCE
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
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

void handler(int s){
  printf("Thread %d, PID %d got signal\n",(int)pthread_self(), (int) getpid());
}

void* thread_func(void* data){
#ifdef III
  printf("Thread %d- captures signal\n",(int) pthread_self());
  signal(SIG,handler);
#endif
#ifdef IV
  printf("Thread %d- blocks signal\n",(int) pthread_self());
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIG);
  sigprocmask(SIG_SETMASK, &sigset, NULL);
#endif

  while(1)
    pause();
 return NULL;
}


int main(){
  pthread_t t;

  pthread_create(&t,NULL,thread_func,NULL);

  printf("Moj PID %d\n",getpid());
#ifdef II
  printf("Main thread %d- blocks signal\n",(int) pthread_self());
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIG);
  sigprocmask(SIG_SETMASK, &sigset, NULL);
#endif
#ifdef III
  printf("Main thread %d- captures signal\n",(int) pthread_self());
  signal(SIG,handler);
#endif

  sleep(1);
  
#if defined(I) || defined(II) || defined(III)
  printf("Main thread %d- sends signal to own process\n",(int) pthread_self());
  raise(SIG);
#endif
  
#if defined(IV) || defined(V)
  printf("Main thread %d- sends signal to thread %d\n",(int) pthread_self(), (int) t);
  pthread_kill(t,SIG);
#endif

  
  

  pthread_join(t,NULL);
  
  return 0;
}


