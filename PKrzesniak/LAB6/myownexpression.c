#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int pfd1[2], pfd2[2], pfd3[2], pfd4[2];
    char *cmd;
    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1) || (pipe(pfd3) == -1) || (pipe(pfd4) == -1)) {
        perror("Pipe error! ");
        exit(EXIT_FAILURE);
    }
    switch (fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
	    cmd = "who";
            char * myargv[2];
            myargv[0] = "who";
	    myargv[1] = NULL;
            dup2(pfd1[1], STDOUT_FILENO);
            close(pfd1[0]);
            close(pfd1[1]);
            execvp(cmd, myargv);
            break;
    }
    switch (fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
	    cmd = "cut";
            char * myargv[4];
            myargv[0] = "cut";
	    myargv[1] = "-d ";
	    myargv[2] = "-f1";
	    myargv[3] = NULL;
            dup2(pfd1[0], STDIN_FILENO);
            dup2(pfd2[1], STDOUT_FILENO);
            close(pfd1[1]);
            close(pfd2[0]);
            close(pfd1[0]);
            close(pfd2[1]);
            execvp(cmd, myargv);
            break;
    }
    close(pfd1[0]);
    close(pfd1[1]);
    switch (fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
 	    cmd = "sort";
            char * myargv[2];
            myargv[0] = "sort";
	    myargv[1] = NULL;
            dup2(pfd2[0], STDIN_FILENO);
            dup2(pfd3[1], STDOUT_FILENO);
            close(pfd2[1]);
            close(pfd3[0]);
            close(pfd2[0]);
            close(pfd3[1]);
            execvp(cmd, myargv);
            break;
    }
    close(pfd2[1]);
    close(pfd2[0]);
    switch (fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:

	    cmd = "uniq";
            char * myargv[3];
            myargv[0] = "uniq";
	    myargv[1] = "-c";
	    myargv[2] = NULL;
            dup2(pfd3[0], STDIN_FILENO);
            dup2(pfd4[1], STDOUT_FILENO);
            close(pfd3[1]);
            close(pfd4[0]);
            close(pfd3[0]);
            close(pfd4[1]);
            execvp(cmd, myargv);
            break;
    }
    close(pfd3[1]);
    close(pfd3[0]);
    dup2(pfd4[0], STDIN_FILENO);
    close(pfd4[1]);
    close(pfd4[0]);
    cmd = "sort";
    char * myargv[3];
    myargv[0] = "sort";
    myargv[1] = "-r";
    myargv[2] = NULL;
    execvp(cmd, myargv);

    return 0;
}
