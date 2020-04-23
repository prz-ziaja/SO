#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]){

	int pfd[2];
	size_t nread;
	char buf[100];

	pipe(pfd);
	char * str ="Ur beautiful pipe example";
	int pid = fork();
	if( pid == -1)
		perror("Fork Error:");
		exit(0);
	else if ( pid == 0){
		close(pfd[1]);
		nread = read(pfd[0], buf, sizeof(buf));
		(nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
	}
	else{
		close(pfd[0]);
		write(pfd[1], str, strlen(str));
	}
	return 0;
}
