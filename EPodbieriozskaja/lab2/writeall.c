#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

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

ssize_t readall(int fd, void *buf, size_t count)
{
    ssize_t n_read = 0, n;

    do{
        if((n=read(fd, &((char *)buf)[n_read], count - n_read))==-1){
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        if(n==0){
            return n_read;
        }
        n_read += n;
    }while(n_read<count);
    return n_read;
}
