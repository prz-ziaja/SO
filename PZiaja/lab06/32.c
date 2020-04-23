#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int pfd1[2], pfd2[2];
    char buf1[100]={0}, buf2[100]={0}, buf3[2]={0};
    int pid1, pid2, i, put;
    size_t nread;

    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1)) {
        perror("Blad przy tworzeniu pipe'ow\n");
        exit(0);
    }

    switch (pid1 = fork()) {
        case -1:
            exit(0);
        case 0:
            close(pfd1[0]);
            close(pfd2[0]);
            close(pfd2[1]);
            close(STDOUT_FILENO);
            dup2(pfd1[1], STDOUT_FILENO);
            execlp("seq", "seq", "10", NULL);
            break;
        default:
            switch (pid2 = fork()) {
                case -1:
                    exit(0);
                case 0:
		    close(pfd1[1]);
		    close(pfd2[0]);
		    read(pfd1[0], buf1, sizeof(buf1));
		    for(int i=0; i<20; i+=2){
	        	sprintf(buf3, "%c%c", buf1[i],buf1[i+1]);
		        put = snprintf(buf2, sizeof(buf2), "%d\n", atoi(buf3) * 5);
		        write(pfd2[1], buf2, put);}
                    break;
                default:
                    close(pfd2[1]);
		    close(pfd1[0]);
		    close(pfd1[1]);
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0);
                    while ((nread = read(pfd2[0], buf2, 100)) > 0) {
                        write(STDOUT_FILENO, buf2, nread);
                    }
                    break;
            }
    }

    return 0;
}
