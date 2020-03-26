#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 64

int main(int argc, char **argv) {
    ssize_t bytes;

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    char buf[BUFSIZE];

    while ((bytes = read(fd, buf, BUFSIZE)) > 0) {
        if(bytes == -1) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }
        buf[bytes] = '\0';
        printf("%s", buf);
    }

    if(close(fd == -1)) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
}