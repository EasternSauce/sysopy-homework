#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[]){
	if(argc < 2){
		printf("Needs one argument\n");
		return -1;
	}
	FILE * pipe = popen("ls -l | grep ^d", "r");
	FILE * file = fopen(argv[1], "w");
	char buffer[1024];
	while(fgets(buffer, 1024, pipe) != NULL){
		fprintf(file, "%s", buffer);
	}

	fclose(pipe);
	fclose(file);

	return 0;
}
