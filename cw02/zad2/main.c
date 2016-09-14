#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <ftw.h>



void find_dir(char * dir, char * permissions){
	DIR * handle;
	struct dirent * entries;

	handle = opendir(dir);

	while((entries = readdir(handle))){
		char file_path[255];
		char file_permissions[3];

		snprintf(file_path, sizeof(file_path), "%s/%s", dir, entries->d_name);

		struct stat file_stat;
		stat(file_path, &file_stat);

		sprintf(file_permissions, "%o", file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
		
		if(entries->d_type == DT_DIR){
			if(strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0){
				find_dir(file_path, permissions);
			}
		}
		else if(entries->d_type == DT_REG && strcmp(permissions, file_permissions) == 0){
			struct tm file_time;
			char last_accessed[255];
			localtime_r(&file_stat.st_atime, &file_time);
			strftime(last_accessed, sizeof(last_accessed), "%c", &file_time);
			
			printf("%s %d bytes, last accessed %s\n", entries->d_name, (int)file_stat.st_size, last_accessed);
		}

	}
	closedir(handle);

}

char * permission_argument;
int process_file(const char * dir, const struct stat * file_stat, int flag){
	char file_permissions[3];
	sprintf(file_permissions, "%o", file_stat->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
	if(strcmp(file_permissions, permission_argument) == 0 && flag == FTW_F){
		char file_path[255];
		realpath(dir, file_path);
		struct tm file_time;
		char last_accessed[255];
		localtime_r(&file_stat->st_atime, &file_time);
		strftime(last_accessed, sizeof(last_accessed), "%c", &file_time);
		
		printf("%s %d bytes, last accessed %s\n", file_path, (int)file_stat->st_size, last_accessed);

	}

	return 0;
}

void nftw_find_dir(char * dir){
	ftw(dir, process_file, FTW_F);
}

int main(int argc, char * argv[]){
	if(argc < 3){
		printf("Needs two arguments\n");
		return -1;
	}

	permission_argument = argv[2];
	printf("non nftw\n");
	find_dir(argv[1], argv[2]);
	printf("nftw\n");
	nftw_find_dir(argv[1]);


}


