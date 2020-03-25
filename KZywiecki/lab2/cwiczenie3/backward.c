/*[backward]*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void backward(char *path)
{
    char s[256], c;
    int i, fd;
    off_t where;
    fd = open(path, O_RDONLY);
    where = lseek(fd, -1, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    i = where;
    int fIterator = 0;
    s[where + 1] = '\0';
    do {
        switch (pread(fd, &c, 1, fIterator)) {
            case 1:
                if (i <= 0) {
                    errno = E2BIG;
                    /* handle the error */
                }
                s[i] = c;
                i--;
                fIterator++;
                break;
            case -1:
                printf(stderr, strerror(errno));
                break;
            default:
                errno = 0;
                /* handle the error */
        }
    } while (i >= 0);
    printf("%s", s);
    close(fd);
    return;
}

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        backward(argv[1]); 
    }
}