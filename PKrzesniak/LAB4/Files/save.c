/* modified by wukat */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include <stdio.h>

#define BUFSIZE 5

int main(int argc, char* argv[]) {
    int bytes = 0, towrite, smin = 0, smax = 10000, sdef = 100, wrote, fd, s =
            sdef;
    struct rlimit limit;
    char buf[BUFSIZE] = {'a', 'a', 'a', 'a', 'a'};
    char* filename;
    if (argc == 1) {
        printf("Use program with a parameter - file path.");
        return 1;
    }
    if (argc == 2) {
        filename = argv[1];
    } else if (argc == 3) {
        filename = argv[2];
        s = atoi(argv[1]);
    } else {
        printf("Too many parameters - use max two");
        return 2;
    }

    if ((s < smin) || (s > smax))
        s = sdef;

    /* reading file size limit */
    getrlimit(RLIMIT_FSIZE, &limit);
    printf("RLIMIT_FSIZE: cur=%d, max=%d\n", (int) limit.rlim_cur,
            (int) limit.rlim_max);
    fflush(stdout);

    printf("Writing %d bytes into %s file...\n", s, filename);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    do {
        towrite = s - bytes;
        towrite = towrite < BUFSIZE ? towrite : BUFSIZE;
        wrote = write(fd, buf, towrite);
        bytes += wrote;
        if (wrote == -1)
            break;
    } while ((wrote > 0) && (bytes < s));
    close(fd);
    return 0;
}
