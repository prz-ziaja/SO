#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define BUFSIZE 512

ssize_t writeall(int fd, const void *buf, size_t nbyte) {
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

void copy(char *from, char *to)
{
    int fromfd = -1, tofd = -1;
    ssize_t nread;
    char buf[BUFSIZE];

    fromfd = open(from, O_RDONLY);
    if (fromfd == -1) {
        if(errno == ENOENT) {
            perror("ERROR"); //No such file or directory
        } else if (errno == EACCES) {
            perror("ERROR"); //Permission denied
        }
        exit(EXIT_FAILURE);
    }

    tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (tofd == -1) {
        if(errno == ENOENT) {
            perror("ERROR"); //No such file or directory
        } else if (errno == EACCES) {
            perror("ERROR"); //Permission denied
        }
        exit(EXIT_FAILURE);
    }

    while((nread = read(fromfd, buf, BUFSIZE)) > 0){
        if (writeall(tofd, buf, nread) == -1){
            perror("ERROR");
            exit(EXIT_FAILURE);
        }
    }

    close(fromfd);
    close(tofd);
}

int main(int argc, char **argv){
    if (argc != 3)
    {
        fprintf(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);
        return 1;
    }
    copy(argv[1], argv[2]);
    return 0;
}
