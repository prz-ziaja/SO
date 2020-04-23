#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int pfd1[2], pfd2[2];
    char buf1[100], buf2[100], buf3[2], buf4[20];
    int pid1, pid2, i, put;
    size_t nread;

    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1)) {
        perror("Error pipe");
        exit(EXIT_FAILURE);
    }

    switch (pid1 = fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
            close(pfd1[0]);
            close(STDOUT_FILENO);
            dup2(pfd1[1], STDOUT_FILENO);
            execlp("seq", "seq", "10", NULL);
            break;
        default:
            switch (pid2 = fork()) {
                case -1:
                    exit(EXIT_FAILURE);
                case 0:
                    close(pfd1[1]);
                    close(pfd2[0]);
                    read(pfd1[0], buf1, 100);
                    for (i = 0; i < 20; i += 2) {
                        buf3[0] = buf1[i];
                        buf3[1] = buf1[i + 1];
                        put = snprintf(buf4, 20, "%d\n", atoi(buf3) * 5);
                        if (i == 18) {
                            buf3[put++] = '\0';
                        }
                        write(pfd2[1], buf4, put);
                    }
                    close(pfd2[1]);
                    break;
                default:
                    close(pfd2[1]);
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0);
                    while ((nread = read(pfd2[0], buf2, 100)) > 0) {
                        write(STDOUT_FILENO, buf2, nread);
                    }
                    break;
            }
    }

    return 0;
}
