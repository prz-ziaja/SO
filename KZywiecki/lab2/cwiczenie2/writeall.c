/*[writeall]*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 512

ssize_t writeall(int fd, const void *buf, size_t nbyte)
{
    ssize_t nwritten = 0, n;
    
    do {
        if ((n = write(fd, ((const char *)buf + nwritten), nbyte - nwritten)) == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        nwritten += n;
    } while (nwritten < nbyte);
    return nwritten;
}

ssize_t readall(int fd, void* buf, size_t nbytes)
{
    ssize_t n = read(fd, buf, nbytes);
    if(n == -1)
    {
        fprintf(stderr, strerror(errno));
        return -1;
    }
    else
    {
        return n;
    }   
}

int main(int argc, char** argv)
{
    char buf[BUFSIZE];
    if(argc == 1)
    {
        writeall(1, buf, readall(2, buf, BUFSIZE));
    }
    else if(argc == 2)
    {
        int fd = open(argv[1], O_RDONLY);
        writeall(1, buf, readall(fd, buf, BUFSIZE));
    }
    else
    {
        fprintf(stdout, "Niepoprawna ilosc argumentow\n");
        return -1;
    }
}