//
// Created by Maryna Lukachyk on 2020-03-25.
//


#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <errno.h>

void backward(char *path)
{
    char s[256], c;
    int i, fd;
    off_t tail;

    fd = open(path, O_RDONLY);
    tail = lseek(fd, 1, SEEK_END);
    i = sizeof(s) - 1;
    s[i] = '\0';

    while (tail > 0) {
        switch (pread(fd, &c, 1, tail - 1)) {
            case 1:
                if (c == '\n') {
                    printf("%s", &s[i]);
                    i = sizeof(s) - 1;
                }
                if (i <= 0) {
                    errno = E2BIG;
                    /* handle the error */
                }
                s[--i] = c;
                break;
            case -1:
                /* handle the error */
                break;
            default: /* impossible */
                errno = 0;
                /* handle the error */
        }

        tail--;
    }
    printf("%s", &s[i]);
    close(fd);
    return;
}

int main(int argc, char* argv[]) {
    backward(argv[1]);

    return 0;
}