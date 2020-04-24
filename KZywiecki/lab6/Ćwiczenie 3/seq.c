#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 128

int pfd1[2];
int pfd2[2];

char buf1[BUFSIZE];
char buf2[BUFSIZE];
char buf3[BUFSIZE];
char buf4[BUFSIZE];

int pid1;
int pid2;

int nread;

int main(int argc, char *argv[]) 
{
    int i;
    int index = 0;

    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1)) {
        printf("Blad przy tworzeniu pipe");
        return -1;
    }

    pid1 = fork();
    switch (pid1) {
        case -1:
            return -1;
            break;
        case 0:
            close(pfd1[0]);
            close(STDOUT_FILENO);
            dup2(pfd1[1], STDOUT_FILENO);
            execlp("seq", "seq", "10", NULL);
            break;
        default:
            pid2 = fork();
            switch (pid2) 
            {
                case -1:
                    return -1;
                case 0:
                    close(pfd1[1]);
                    close(pfd2[0]);
                    read(pfd1[0], buf1, BUFSIZE);
                    for (i = 0; i < 10; i++) 
                    {
                        buf3[0] = buf1[i * 2];
                        buf3[1] = buf1[i * 2 + 1];
                        index = snprintf(buf4, BUFSIZE, "%d\n", atoi(buf3) * 5);
                        if (i == 9) 
                        {
                            buf3[index] = '\0';
                            index++;
                        }
                        write(pfd2[1], buf4, index);
                    }
                    close(pfd2[1]);
                    break;
                default:
                    close(pfd2[1]);
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0);
                    while ((nread = read(pfd2[0], buf2, BUFSIZE)) > 0) 
                    {
                        write(STDOUT_FILENO, buf2, nread);
                    }
                    break;
            }
    }

    return 0;
}