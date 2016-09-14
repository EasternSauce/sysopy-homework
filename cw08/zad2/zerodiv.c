#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>



void * zero_div(void* unused){
  int a=1;
  int b=0;
  int c;
  while(1){
    puts("Div by 0");
    c = a/b;
    printf("%d\n", c);
  }
}

void * regular_thread(void* unused){
  while(1){  
    printf("running");
  }
}


int main(){
  pthread_t w1,w2,w3;

  pthread_create(&w1,NULL,regular_thread,NULL);
  pthread_create(&w2,NULL,regular_thread,NULL);
  pthread_create(&w3,NULL,zero_div,NULL);

  pthread_join(w1,NULL);
  pthread_join(w2,NULL);
  pthread_join(w3,NULL);

  return 0;
}

