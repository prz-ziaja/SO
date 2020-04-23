#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void){
	int pfd_1[2], pfd_2[2];
	int fd, pid_1, pid_2, put;
	size_t nread;
	char buf1[20], buf2[20], buf3[3];

	if((pipe(pfd_1)) == -1 || (pipe(pfd_2) == -1)){
		perror("Pipe Error:");
		exit(EXIT_FAILURE);
	}

	pid_1 = fork();
	if(pid_1 == -1){
		perror("Fork Error:");
		exit(EXIT_FAILURE);
	}
	else if(pid_1 == 0){
		close(pfd_1[0]);
		close(STDOUT_FILENO);
		dup2(pfd_1[1], STDOUT_FILENO);
		execlp("seq", "seq", "10", NULL);
	}
	else{
		pid_2 = fork();
		if(pid_2 == -1){
			perror("Fork Error:");
			exit(EXIT_FAILURE);
		}
		else if(pid_2 == 0){
			close(pfd_1[1]);
			close(pfd_2[0]);
			read(pfd_1[0], buf1, 20);
			for(int i=0; i<20; i+=2){
				sprintf(buf3, "%c%c", buf1[i],buf1[i+1]);
		                put = snprintf(buf2, 20, "%d\n", atoi(buf3) * 5);
		                write(pfd_2[1], buf2, put);
			}
		close(pfd_2[1]);
		}
		else{
			close(pfd_2[1]);
			waitpid(pid_1, NULL, 0);
			waitpid(pid_2, NULL, 0);
			while((nread = read(pfd_2[0], buf1, 20)) > 0){
				write(STDOUT_FILENO, buf1,nread);
			}
		}
		
	}
	return 0;
}

