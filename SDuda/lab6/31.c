#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void){
	int pfd_1_f[2], pfd_1_b[2], pfd_2_f[2], pfd_2_b[2];
	int fd, pid_1, pid_2;
	size_t nread;
	char buf[256];

	if((pipe(pfd_1_f)) == -1 || (pipe(pfd_1_b) == -1) || (pipe(pfd_2_f) == -1) || (pipe(pfd_2_b) == -1)){
		perror("Error has occured:");
		exit(EXIT_FAILURE);
	}

	fd = open("dictionary.txt", O_RDONLY);
	if(fd == -1){
		perror("Open Error:");
		close(fd);
		exit(EXIT_FAILURE);
	}

	pid_1 = fork();
	if(pid_1 == 0){
		close(pfd_1_f[1]);
		close(pfd_1_b[0]);

		pid_2 = fork();
		if(pid_2 == 0){
			close(pfd_2_f[1]);
			close(STDIN_FILENO);
			dup2(pfd_2_f[0], STDIN_FILENO);
			close(pfd_2_b[0]);
			close(STDOUT_FILENO);
			dup2(pfd_2_b[1], STDOUT_FILENO);
			execlp("grep", "grep", "-c", "pipe", NULL);
		}
		else if(pid_2 == -1){
			perror("Fork Error:");
			close(fd);
			exit(EXIT_FAILURE);
		}

		close(STDIN_FILENO);
		dup2(pfd_1_f[0], STDIN_FILENO);
		close(STDOUT_FILENO);
		dup2(pfd_1_b[1], STDOUT_FILENO);
		execlp("wc", "wc", "-l", NULL);
	}
	else if(pid_1 == -1){
		perror("Fork Error:");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(pfd_1_f[0]);
	close(pfd_2_f[0]);
	while((nread = read(fd, buf, sizeof(buf))) > 0){
		if(write(pfd_1_f[1], buf, nread) == -1){
			perror("Comunication Error:");
			close(fd);
			exit(EXIT_FAILURE);
		};
		if(write(pfd_2_f[1], buf, nread) == -1){
			perror("Comunication Error:");
                        close(fd);
                        exit(EXIT_FAILURE);
		};
	}

	close(pfd_1_f[1]);
    	close(pfd_2_f[1]);
	close(pfd_2_b[1]);
	read(pfd_2_b[0], buf, sizeof(buf));
	printf("Liczba slow z 'pipe': %d \n", atoi(buf));
	close(pfd_1_b[1]);
	read(pfd_1_b[0], buf, sizeof(buf));
	printf("Liczba wszystkich slow: %d \n", atoi(buf));
	close(fd);

	return 0;
}