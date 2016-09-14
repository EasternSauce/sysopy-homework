#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char * argv[]){
	if(argc!=2){
		printf("Needs one argument\n");
		return -1;
	}
	
	int fifo = open(argv[1], O_RDONLY);

	mkfifo(argv[1], 0666);

	char buffer[1024];

	int bytes_read;
	while((bytes_read = read(fifo, buffer, 1024))){
		buffer[bytes_read] = 0;
		time_t raw_time;
		time(&raw_time);
		struct tm * time_info = localtime(&raw_time);
		printf("%d:%d:%d - %s", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, buffer);
	}

	close(fifo);
	unlink(argv[1]);

	return 0;
}
