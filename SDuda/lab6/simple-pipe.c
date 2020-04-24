#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]){

int pfd[2];
size_t nread;
char buf[100]={0};
int pid;
pipe(pfd);
char * str ="Ur beautiful pipe example";
switch(pid=fork()){
	case -1:
		printf("Blad przy tworzeniu procesu\n");
		exit(0);
	case 0:
		close(pfd[0]);
		write(pfd[1], str, strlen(str));
		break;
	default:
		close(pfd[1]);
		nread=read(pfd[0],buf, sizeof(buf));
		(nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
}

return 0;
}