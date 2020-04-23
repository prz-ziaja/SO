#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "stdlib.h"

char *my_itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}
int main(int argc, char *argv[]){

FILE * fp;
char * line = NULL;
size_t len = 0;
ssize_t readd;

int i=0;
int pfd[2];
int rfd[2];
int sfd[2];
int qfd[2];
size_t nread;
char buf[100]={0};
int pid1,pid2;
pipe(pfd);
pipe(rfd);
pipe(sfd);

switch(pid1=fork()){
	case -1:
		printf("Blad przy tworzeniu procesu\n");
		exit(0);
	case 0:
		close(STDIN_FILENO);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);//close write pipe
		close(pfd[1]);
		close(rfd[0]);
		close(rfd[1]);
		close(sfd[0]);
		while ((readd = getline(&line, &len, stdin)) != -1) {		
			i+=1;
		}
		my_itoa (i,buf);		
		write(sfd[1], buf, (strlen(buf)+1));
		close(sfd[1]);
		exit(0);
	default:
		pipe(qfd);
		switch(pid2=fork()){
			case -1:
				printf("Blad przy tworzeniu procesu\n");
				exit(0);
			case 0:
				close(sfd[1]);
				close(sfd[0]);
				close(STDIN_FILENO);
				dup2(rfd[0], STDIN_FILENO);
				close(rfd[0]);
				close(rfd[1]);//close write pipe
				close(pfd[0]);
				close(pfd[1]);
				while ((readd = getline(&line, &len, stdin)) != -1) {
					if(strstr(line, "pipe") != NULL) {
 						i+=1;
					}
				}
				my_itoa (i,buf);
				write(qfd[1], buf, (strlen(buf)+1));
				close(qfd[1]);
				exit(0);
				
			default:
				close(pfd[0]);//close write pipe
				close(rfd[0]);
				close(qfd[1]);
				close(sfd[1]);
				fp = fopen("dictionary.txt", "r");
				if (fp == NULL){
					printf("Blad pliku\n");
					exit(EXIT_FAILURE);
				}
				while ((readd = getline(&line, &len, fp)) != -1) {
					write(pfd[1], line, readd);
					write(rfd[1], line, readd);
				}
				fclose(fp);
				if (line)
					free(line);
				close(pfd[1]);//close write pipe
				close(rfd[1]);
				waitpid(pid1, NULL, 0);
				waitpid(pid2, NULL, 0);
				read(sfd[0], buf, sizeof (buf));
    				printf("Liczba linii: %s \n", buf);
				read(qfd[0], buf, sizeof (buf));
    				printf("Liczba linii z pipe: %s \n", buf);
	}
}

return 0;
}
