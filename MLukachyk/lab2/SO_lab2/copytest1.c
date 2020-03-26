//
// Created by Maryna Lukachyk on 2020-03-25.
//

#include "t.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define BUFSIZE 512

ssize_t writeall(int fd, const void *buf, size_t nbyte){
    ssize_t nwritten = 0, n;

    do {
        if ((n = write(fd, &((const char *)buf)[nwritten], nbyte - nwritten)) == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        nwritten += n;
    } while (nwritten < nbyte);
    return nwritten;
}

void copy(char *from, char *to){
    int fromfd = -1, tofd = -1;
    ssize_t bytes;
    char buff[BUFSIZE];

    fromfd = open(from, O_RDONLY);

    if(fromfd ==-1){
        if(errno == EACCES){          // EACCES -> flaga bledu Access Error permission denied
            perror("ERROR: Permission Denied!");
        } else if(errno == ENOENT){   // ENOENT -> Error NO ENTity
            perror("ERROR: Source file does not exist!");
        } else {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }
    }

    tofd = creat(to, 0640);
    while((bytes = read(fromfd, buff, BUFSIZE)) > 0){
        if (writeall(tofd, buff, bytes) == -1){
            perror("ERROR: Failed saving to target file!");
            exit(EXIT_FAILURE);
        }
    }

    if (bytes == -1){
        perror("ERROR: Failed reading source file!");
        exit(EXIT_FAILURE);
    }

    if (close(fromfd) == -1 || close(tofd) == -1){
        perror("ERROR: Failed closing files!");
        exit(EXIT_FAILURE);
    }
}

void copy3(char *from, char *to)
{
    FILE *stfrom, *stto;
    int c;

    if ((stfrom = fopen(from, "r") ) == NULL){}
    if(( stto = fopen(to, "w") ) == NULL) {}
    while ((c = getc(stfrom)) != EOF)
        putc(c, stto);
    fclose(stfrom);
    fclose(stto);
    return;

}

int main(int argc, char *argv[]){

    timestart();
    copy("file1", "file_copy2");
    timestop("Copy 2");

    timestart();
    copy3("file1", "file_copy3");
    timestop("Copy 3");

    return 0;
}