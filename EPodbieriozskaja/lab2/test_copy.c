#include "t.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFSIZE 512

ssize_t writeall(int fd, const void *buf, size_t nbyte)
{
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

void copy(char *from, char *to) {
    int fromfd = -1, tofd = -1;
    ssize_t nread;
    char buf[BUFSIZE];
    errno = 0;

    fromfd = open(from, O_RDONLY);
    if (fromfd==-1) {
        if (errno == ENOENT) {
            perror("No such source file");
        } else if (errno == EACCES) {
            perror("No permissions");
        }
        exit(EXIT_FAILURE);
    }
    tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
                S_IRUSR | S_IWUSR);
    while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
        if (writeall(tofd, buf, nread)==-1) {
            perror("Error while saving to target file");
            exit(EXIT_FAILURE);
        }

    if (close(fromfd)==-1 || close(tofd)==-1) {
        perror("Error while closing file");
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
}


int main(int argc, char **argv) {
    timestart();
    copy("1.txt", "2.txt");
    timestop("copy 2");

    timestart();
    copy3("1.txt", "3.txt");
    timestop("copy 3");

    return 0;
}

//output:
//copy 2:
//"Total (user/sys/real)", 0, 3, 6
//"Child (user/sys)", 0, 0
//copy 3:
//"Total (user/sys/real)", 5, 1, 8
//"Child (user/sys)", 0, 0
