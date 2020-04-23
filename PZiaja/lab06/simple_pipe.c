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
		close(pfd[0]);//zamykam pipe do czytania, poniewaz w procesie dziecku bede do niego tylko zapisywal
		write(pfd[1], str, strlen(str));
		break;
	default:
		close(pfd[1]);//zamykam pipe do zapisu, poniewaz proces rodzic bedzie tylko sluchal
		nread=read(pfd[0],buf, sizeof(buf));
		(nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
		printf("%ld\n",fpathconf(pfd[0], _PC_PIPE_BUF));//jezeli do pipe write zapisze na raz wiecej niz PIPE_BUF bajtow moga sie pojawic problemy z zapisem (uporzadkowanie bajtow lub nawet brak zapisu do pipe'a)
}

return 0;
}
