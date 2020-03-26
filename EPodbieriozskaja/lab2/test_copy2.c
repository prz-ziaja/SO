#include "t.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>


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

void copy(char *from, char *to, int size_buf) {
    int fromfd = -1, tofd = -1;
    ssize_t nread;
    char buf[size_buf];
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


int main(int argc, char **argv) {
    timestart();
    copy("1.txt", "2.txt", 1);
    timestop("copy size_buf=1");

    timestart();
    copy("1.txt", "2.txt", 512);
    timestop("copy size_buf=512");

    timestart();
    copy("1.txt", "2.txt", 1024);
    timestop("copy size_buf=1024");

    timestart();
    copy("1.txt", "2.txt", 1100);
    timestop("copy size_buf=1100");

    return 0;
}


//    output:
//copy size_buf=1:
//"Total (user/sys/real)", 189, 1169, 1359
//"Child (user/sys)", 0, 0
//copy size_buf=512:
//"Total (user/sys/real)", 0, 3, 6
//"Child (user/sys)", 0, 0
//copy size_buf=1024:
//"Total (user/sys/real)", 0, 2, 4
//"Child (user/sys)", 0, 0
//copy size_buf=1100:
//"Total (user/sys/real)", 1, 2, 5
//"Child (user/sys)", 0, 0
