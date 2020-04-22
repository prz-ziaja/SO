#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]){

int pfd[2];
size_t nread;
char buf[100];
char * str ="Ur beautiful pipe example";

pid_t child_pid;

pipe(pfd);

child_pid = fork();
switch(child_pid)
{
	case -1: /* error */
		perror("fork");
		return 1;
		break;
	case 0: /* child - reading from pipe */
		close(pfd[1]); /* closing unused write end */
		nread=read(pfd[0],buf, sizeof(buf)); /* blocks thread */
		(nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
		close(pfd[0]);
		break;
	default: /* parent - writing to pipe */
		close(pfd[0]); /* closing unused read end */
		write(pfd[1], str, strlen(str));
		close(pfd[1]);
		wait(NULL); /* wait for child */
		printf("fpathconf for read:%ld write:%ld\n",fpathconf(pfd[0],_PC_PIPE_BUF),fpathconf(pfd[1],_PC_PIPE_BUF));
		break;
}


return 0;
}
