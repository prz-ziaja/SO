#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


void tail(char *path){
    char c;
    int fd, cline;
    off_t tail, end;

    fd = open(path, O_RDONLY);
    end = lseek(fd, 1, SEEK_END);
    tail =end;
    cline=0;

    while(cline<11 && tail>0){
        if (pread(fd, &c, 1, tail-1) == -1) {
            perror("Fail");
            exit(EXIT_FAILURE);
        }
        if(c == '\n'){
            cline++;
        }
        if(cline == 11){
            tail ++;
        }
        tail --;
    }

    while (tail < end) {
        if (pread(fd, &c, 1, tail) == -1) {
            perror("Fail");
            exit(EXIT_FAILURE);
        }
        putchar(c);
        tail++;
    }

    if (close(fd)==-1) {
        perror("Error while closing file");
        exit(EXIT_FAILURE);
    }

}


int main(int argc, char *argv[]){
    tail(argv[1]);
    return 0;
}
