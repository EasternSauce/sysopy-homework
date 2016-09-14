#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
	if(argc!=2){
		printf("Needs one argument\n");
		return -1;
	}
	
	int fifo = open(argv[1], O_WRONLY);

	size_t size;
	char * line = NULL;
	while(getline(&line, &size, stdin)){
		char buffer[1024];
		time_t raw_time;
		time(&raw_time);
		struct tm * time_info = localtime(&raw_time);

		sprintf(buffer, "%d - %d:%d:%d - %s", getpid(), time_info->tm_hour, time_info->tm_min, time_info->tm_sec, line);

		write(fifo, buffer, strlen(buffer));
	}

	close(fifo);

	return 0;
}
