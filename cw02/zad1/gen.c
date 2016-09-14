#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

void generate(char* file_name, int record_size, int file_size){
	int handle = open(file_name, O_WRONLY | O_CREAT, 0644);
	srand(time(NULL));

	for(int i = 0; i < file_size; i++){
		for(int j = 0; j < record_size; j++){
			unsigned char buffer = rand()%255;
			write(handle, &buffer, 1); 			
		}
	}

	close(handle);
}

int main(int argc, char *argv[]){
	if(argc != 4) {printf("Needs three arguments and not %d\n", argc); exit(1);}
	char * file_name = argv[1];
	int record_size = atoi(argv[2]);
	int file_size = atoi(argv[3]);

	generate(file_name, record_size, file_size);
	
	return 0;
}

