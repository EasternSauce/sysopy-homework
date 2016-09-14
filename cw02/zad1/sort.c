#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



void print_file(char* file_name, int record_size, int file_size){
	int handle = open(file_name, O_RDWR, 0644);

	for(int i = 0; i < file_size; i++){
		unsigned char * buffer = malloc(sizeof(unsigned char) * record_size);

		read(handle, buffer, record_size); 			

		for(int j = 0; j < record_size; j++){
			printf("%d ", buffer[j]);
		}
		printf("\n");
	}
	close(handle);
}


void sort_file(char* file_name, int record_size, int file_size){
	int handle = open(file_name, O_RDWR, 0644);

	for(int i = 0; i < file_size; i++){
		unsigned char * max_record = malloc(sizeof(unsigned char) * record_size);
		max_record[0] = 0;
		int max_pos = i;

		lseek(handle, i*record_size, SEEK_SET);
		int swapped = 0;
		for(int j = i; j < file_size; j++){
			unsigned char * buffer = malloc(sizeof(unsigned char) * record_size);

			read(handle, buffer, record_size); 			

			if(buffer[0] > max_record[0]){
				memcpy(max_record, buffer, sizeof(unsigned char) * record_size);
				max_pos = j;
				swapped = 1;
			}
		}
		if(!swapped) break;

		for(int i = max_pos; i > 0; i--){
			lseek(handle, (i-1)*record_size, SEEK_SET);
			unsigned char * buffer = malloc(sizeof(unsigned char) * record_size);
			read(handle, buffer, record_size);
			lseek(handle, i*record_size, SEEK_SET);
			write(handle, buffer, record_size);
		}

		lseek(handle, 0, SEEK_SET);
		
		write(handle, max_record, record_size);
	}

	close(handle);
}

void lib_sort_file(char* file_name, int record_size, int file_size){
	FILE* handle = fopen(file_name, "rw+");

	for(int i = 0; i < file_size; i++){
		unsigned char * max_record = malloc(sizeof(unsigned char) * record_size);
		max_record[0] = 0;
		int max_pos = i;

		fseek(handle, i*record_size, SEEK_SET);
		int swapped = 0;
		for(int j = i; j < file_size; j++){
			unsigned char * buffer = malloc(sizeof(unsigned char) * record_size);

			fread(buffer, 1, record_size, handle);

			if(buffer[0] > max_record[0]){
				memcpy(max_record, buffer, sizeof(unsigned char) * record_size);
				max_pos = j;
				swapped = 1;
			}
		}
		if(!swapped) break;

		for(int i = max_pos; i > 0; i--){
			fseek(handle, (i-1)*record_size, SEEK_SET);
			unsigned char * buffer = malloc(sizeof(unsigned char) * record_size);
			fread(buffer, 1, record_size, handle);
			fseek(handle, i*record_size, SEEK_SET);
			fwrite(buffer, 1, record_size, handle);
		}

		fseek(handle, 0, SEEK_SET);
		
		fwrite(max_record, 1, record_size, handle);
	}

	fclose(handle);
}



int main(int argc, char *argv[]){
	if(argc != 5) {printf("Needs four arguments and not %d\n", argc); exit(1);}
	char * file_name = argv[1];
	int record_size = atoi(argv[2]);
	int file_size = atoi(argv[3]);
	char * stype = argv[4];
	int type = -1;

	if(strcmp(stype,"sys") == 0) type = 0;
	if(strcmp(stype,"lib") == 0) type = 1;

	if(type == 0){
		sort_file(file_name, record_size, file_size);
	}
	else if(type == 1){
		lib_sort_file(file_name, record_size, file_size);
	}







	return 0;
}
