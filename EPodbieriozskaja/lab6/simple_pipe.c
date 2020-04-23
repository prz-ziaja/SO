#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

    int pfd[2];
    size_t nread;
    char buf[100];
    int pid;
    long v1, v2;

    pipe(pfd);
    v1 = fpathconf(pfd[0], _PC_PIPE_BUF);
    v2 = fpathconf(pfd[1], _PC_PIPE_BUF);
    printf("buffer size 1 descr: %ld 2 descr: %ld \n", v1, v2);

    char * str ="Ur beautiful pipe example";

    switch (pid = fork()) {
        case -1:
            exit(-1);
        case 0:
            close(pfd[1]);
            nread = read(pfd[0], buf, 100);
            (nread != 0) ? printf("%s (%ld bytes)\n", buf, (long) nread) : printf("No data\n");
            break;
        default:
            close(pfd[0]);
            write(pfd[1], str, strlen(str));
    }


    return 0;
}

