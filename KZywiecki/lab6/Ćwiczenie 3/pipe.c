#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pfd1[2];
int pfd2[2];
int pfd3[2];
int pfd4[2];

int pid1;
int pid2;
int pid3;
int pid4;

int main(int argc, char *argv[]) 
{
    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1) || (pipe(pfd3) == -1) || (pipe(pfd4) == -1)) 
    {
        perror("Blad przy tworzeniu pipe");
        return -1;
    }

    pid1 = fork();
    switch (pid1) 
    {
        case -1:
            return -1;
        case 0:
            dup2(pfd1[1], STDOUT_FILENO);
            close(pfd1[0]);
            close(pfd1[1]);
            execlp("who", "who", NULL);
            break;
    }
    waitpid(pid1, NULL, 0);
    pid2 = fork();
    switch (pid2) 
    {
        case -1:
            return -1;
        case 0:
            dup2(pfd1[0], STDIN_FILENO);
            dup2(pfd2[1], STDOUT_FILENO);
            close(pfd1[1]);
            close(pfd2[0]);
            close(pfd1[0]);
            close(pfd2[1]);
            execlp("cut", "cut", "-d ", "-f1", NULL);
            break;
    }
    close(pfd1[0]);
    close(pfd1[1]);
    waitpid(pid2, NULL, 0);

    pid3 = fork();
    switch (pid3) 
    {
        case -1:
            return -1;
        case 0:
            dup2(pfd2[0], STDIN_FILENO);
            dup2(pfd3[1], STDOUT_FILENO);
            close(pfd2[1]);
            close(pfd3[0]);
            close(pfd2[0]);
            close(pfd3[1]);
            execlp("sort", "sort", NULL);
            break;
    }
    close(pfd2[1]);
    close(pfd2[0]);
    waitpid(pid3, NULL, 0);

    pid4 = fork();
    switch (pid4) 
    {
        case -1:
            return -1;
        case 0:
            dup2(pfd3[0], STDIN_FILENO);
            dup2(pfd4[1], STDOUT_FILENO);
            close(pfd3[1]);
            close(pfd4[0]);
            close(pfd3[0]);
            close(pfd4[1]);
            execlp("uniq", "uniq", "-c", NULL);
            break;
    }
    close(pfd3[1]);
    close(pfd3[0]);
    waitpid(pid4, NULL, 0);

    dup2(pfd4[0], STDIN_FILENO);
    close(pfd4[1]);
    close(pfd4[0]);
    execlp("sort", "sort", "-r", NULL);

    return 0;
}