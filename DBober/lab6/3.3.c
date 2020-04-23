#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int pfd_1[2], pfd_2[2], pfd_3[2], pfd_4[2];
	int pid_1, pid_2, pid_3, pid_4;
	
	if(pipe(pfd_1) == -1 || pipe(pfd_2) == -1 || pipe(pfd_3) == -1 || pipe(pfd_4) == -1) {
		perror("Pipe Error:");
		exit(EXIT_FAILURE);
	}

	pid_1 = fork();
	if(pid_1 == -1){
		perror("Fork Error:");
		exit(EXIT_FAILURE);
	}
	if(pid_1 == 0){
		close(pfd_1[0]);
		close(STDOUT_FILENO);
		dup2(pfd_1[1], STDOUT_FILENO);
		execlp("who", "who", NULL);
	}

	waitpid(pid_1, NULL, 0);
	pid_2 = fork();
	if(pid_2 == -1){
                perror("Fork Error:");
                exit(EXIT_FAILURE);
        }
        if(pid_2 == 0){
		close(pfd_1[1]);
                close(pfd_2[0]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		dup2(pfd_1[0], STDIN_FILENO);
                dup2(pfd_2[1], STDOUT_FILENO);
                execlp("cut", "cut", "-d ", "-f1", NULL);
        }
	close(pfd_1[1]);
        close(pfd_1[0]);

        waitpid(pid_2, NULL,  0);
	pid_3 = fork();
        if(pid_3 == -1){
                perror("Fork Error:");
                exit(EXIT_FAILURE);
        }
        if(pid_3 == 0){
		close(pfd_2[1]);
                close(pfd_3[0]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
                dup2(pfd_2[0], STDIN_FILENO);
                dup2(pfd_3[1], STDOUT_FILENO);
                execlp("sort", "sort", NULL);
        }
        close(pfd_2[1]);
        close(pfd_2[0]);

        waitpid(pid_3, NULL,  0);
	pid_4 = fork();
	if(pid_4 == -1){
                perror("Fork Error:");
                exit(EXIT_FAILURE);
        }
        if(pid_4 == 0){
		close(pfd_3[1]);
                close(pfd_4[0]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
                dup2(pfd_3[0], STDIN_FILENO);
                dup2(pfd_4[1], STDOUT_FILENO);
                execlp("uniq", "uniq", "-c", NULL);
        }
	close(pfd_3[1]);
	close(pfd_3[0]);

	waitpid(pid_4, NULL, 0);
	if(pid_1 + pid_2 + pid_3 + pid_4 > 4){
		close(pfd_4[1]);
		close(STDIN_FILENO);
		dup2(pfd_4[0], STDIN_FILENO);
		execlp("sort", "sort", "-r", NULL);
	}
	return 0;
}
