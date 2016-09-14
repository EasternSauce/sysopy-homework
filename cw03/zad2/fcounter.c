#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define PATH_LENGTH 256
#define SLEEPS_FOR 15

int main(int argc, char * argv[]){
	int file_count = 0;
	int files_in_dir_count = 0;
	int proc_count = 0;
	int sleeps = 0;

	char path[PATH_LENGTH];
	if(argc == 1){
		strcpy(path, ".");
	}
	else if(argc == 2){
		if(strcmp(argv[1], "-w") == 0){
			sleeps = 1;
			strcpy(path, ".");
		}
		else strcpy(path, argv[1]);
	}
	else if(argc == 3){
		strcpy(path, argv[1]);
		if(strcmp(argv[2], "-w") == 0) sleeps = 1;
	}
	else{
		printf("needs directory argument and optionally -w argument to wait 15 seconds\n");
		return 0;
	}

	DIR * dir = opendir(path);
	
	printf("Currently in %s\n", path);

	struct dirent * entries;
	while((entries = readdir(dir)) != NULL){
		if(strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0){
			char deeper_path[PATH_LENGTH];
			strcpy(deeper_path, path);
			strcat(deeper_path, "/");
			strcat(deeper_path, entries->d_name); 

			printf("%s\n", deeper_path);

			struct stat my_stat;
			stat(deeper_path, &my_stat);

			if(S_ISDIR(my_stat.st_mode)){
				proc_count++;

				pid_t pid;
				pid = fork();

				if(pid == 0){
					if(!sleeps) execlp(argv[0], argv[0], deeper_path, NULL);
					else execlp(argv[0], argv[0], deeper_path, "-w", NULL);
				}

			}
			else if(S_ISREG(my_stat.st_mode)){
				file_count++;
			}

		}
	}

	if(sleeps) sleep(SLEEPS_FOR);

	for(int i = 0; i < proc_count; i++){
		int status;
		wait(&status);
		files_in_dir_count += WEXITSTATUS(status);
	}

	_exit(file_count + files_in_dir_count);

}
