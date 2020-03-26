#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>

int BUFSIZE  = 512;

static struct tms tbuf1;
static clock_t real1;
static long clock_ticks;

void timestart(void)
{
    real1 = times(&tbuf1);
    clock_ticks = sysconf(_SC_CLK_TCK);
    return;
}

void timestop(char *msg)
{
    struct tms tbuf2;
    clock_t real2;

    real2 = times(&tbuf2);
    fprintf(stdout, "%s:\n\t\"Total (user/sys/real)\", %ld, %ld, %ld\n\t\"Child (user/sys)\", %ld, %ld\n",
            msg,
            (long)((tbuf2.tms_utime + tbuf2.tms_cutime) - (tbuf1.tms_utime + tbuf1.tms_cutime)),
            (long)((tbuf2.tms_stime + tbuf2.tms_cstime) - (tbuf1.tms_stime + tbuf1.tms_cstime)),
            (long)(real2 - real1),
            (long)(tbuf2.tms_cutime - tbuf1.tms_cutime),
            (long)(tbuf2.tms_cstime - tbuf1.tms_cstime));
    return;
}

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

void copy3(char *from, char *to)
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

    while ((nread = read(fromfd, buf, sizeof(buf))) > 0) {
        if(nread == -1) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }

        int wr_error = write(tofd, buf, nread);
        if (wr_error == -1) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }
    }

    close(fromfd);
    close(tofd);
}

int main(int argc, char **argv){
	BUFSIZE = *argv[1];
    timestart();
    copy("file1.txt", "file2.txt");
    timestop("Copy 2");

    timestart();
    copy3("file1.txt", "file3.txt");
    timestop("Copy 3");
}
