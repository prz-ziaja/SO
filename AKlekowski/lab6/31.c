#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 128

int main(void) {
    int pfd1[2], pfd2[2], pfd1b[2], pfd2b[2];
    pid_t pid1, pid2;
    int nread0 = 0, fd = 0;
    char buf0[BUFSIZE];

    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1) || (pipe(pfd1b) == -1) || (pipe(pfd2b) == -1)) {
        perror("Something went wrong with pipe:");
        exit(EXIT_FAILURE);
    }

    fd = open("dictionary.txt", O_RDONLY);
    pid1 = fork();
    if (pid1 == 0) { /* child 1 */
        close(pfd1[1]);
        close(pfd1b[0]);
        pid2 = fork();
        if (pid2 == 0) { /* child 2 */
            close(pfd2[1]);
            close(pfd2b[0]);
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            dup2(pfd2[0], STDIN_FILENO);
            dup2(pfd2b[1], STDOUT_FILENO);
            execlp("grep", "grep", "-c", "pipe", NULL);
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        dup2(pfd1[0], STDIN_FILENO);
        dup2(pfd1b[1], STDOUT_FILENO);
        execlp("wc", "wc", "-l", NULL);
    }

    close(pfd1[0]);
    close(pfd2[0]);
    while ((nread0 = read(fd, buf0, sizeof (buf0))) > 0) {
        if (write(pfd1[1], buf0, nread0) == -1) {
            perror("1");
        };
        if (write(pfd2[1], buf0, nread0) == -1) {
            perror("2");
        };
    }
    close(pfd1[1]);
    close(pfd2[1]);
    close(pfd1b[1]);
    close(pfd2b[1]);
    read(pfd1b[0], buf0, sizeof (buf0));
    printf("Liczba linii: %d \n", atoi(buf0));
    read(pfd2b[0], buf0, sizeof (buf0));
    printf("Liczba linii z pipe: %d \n", atoi(buf0));
    return 0;
}
