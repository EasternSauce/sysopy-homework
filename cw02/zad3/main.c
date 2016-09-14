#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
	if(argc != 2){
		printf("Needs 1 argument\n");
		return -1;
	}

	char * file_name = argv[1];

	int handle = open(file_name, O_RDWR);

	struct stat file_stat;
	stat(file_name, &file_stat);
	int file_size = (int)file_stat.st_size;


	printf("r - lock reading a character\n");
	printf("w - lock writing a character\n");
	printf("u - unlock a character\n");
	printf("p - print locked characters\n");
	printf("g - get a character\n");
	printf("s - set a character\n");
	printf("q - quit\n");

	int running = 1;
	while(running){
		char option;
		printf("Input: ");
		scanf("%c", &option);
		getchar();

		if(option == 'q'){
			running = 0;
		}
		else if(option == 'p'){
			printf("Locked:\n");
			for(int i = 1; i < file_size; i++){
				struct flock lock;
				lock.l_len = 1;
				lock.l_start = i;
				lock.l_type = F_WRLCK;
				lock.l_whence = SEEK_SET;
				fcntl(handle, F_GETLK, &lock);

				if(lock.l_type != F_UNLCK){
					printf("Index %d locked by pid %d\n", i, lock.l_pid);
				}
			}
		}
		else if(option == 'r'){
			int index;
			printf("Character index: ");
			scanf("%d", &index);
			getchar();

			struct flock lock;
			lock.l_len = 1;
			lock.l_start = index;
			lock.l_type = F_RDLCK;
			lock.l_whence = SEEK_SET;

			int ok = fcntl(handle, F_SETLK, &lock);
			if(ok != -1) printf("Lock set\n");
			else printf("error\n");
		}
		else if(option == 'w'){
			int index;
			printf("Character index: ");
			scanf("%d", &index);
			getchar();

			struct flock lock;
			lock.l_len = 1;
			lock.l_start = index;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;

			int ok = fcntl(handle, F_SETLK, &lock);
			if(ok != -1) printf("Lock set\n");
			else printf("error\n");
		}
		else if(option == 'u'){
			int index;
			printf("Character index: ");
			scanf("%d", &index);
			getchar();

			struct flock lock;
			lock.l_len = 1;
			lock.l_start = index;
			lock.l_type = F_UNLCK;
			lock.l_whence = SEEK_SET;

			int ok = fcntl(handle, F_SETLK, &lock);
			if(ok != -1) printf("Lock removed\n");
			else printf("error\n");

		}
		else if(option == 'g'){
			int index;
			printf("Character index: ");
			scanf("%d", &index);
			getchar();

			char character;
			lseek(handle, index - 1, SEEK_SET);
			read(handle, &character, 1);
			printf("Character is %c\n", character);
		}
		else if(option == 's'){
			int index;
			printf("Character index: ");
			scanf("%d", &index);
			getchar();

			char character;
			printf("The character:");
			scanf("%d", &character);
			getchar();
			lseek(handle, index - 1, SEEK_SET);
			write(handle, &character, 1);

		}
		else{
			printf("Unknown option\n");
		}
	}

	return 0;
}
