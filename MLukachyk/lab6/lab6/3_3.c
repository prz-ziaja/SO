//
// Created by Maryna Lukachyk on 2020-04-23.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int pfd1[2], pfd2[2], pfd3[2], pfd4[2];
    int pid1, pid2, pid3, pid4;

    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1) || (pipe(pfd3) == -1) || (pipe(pfd4) == -1)) {
        perror("Something went wrong with pipe:");
        exit(EXIT_FAILURE);
    }
    switch (pid1 = fork()) {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
            dup2(pfd1[1], STDOUT_FILENO);
            close(pfd1[0]);
            close(pfd1[1]);
            execlp("who", "who", NULL);
            break;
    }
    waitpid(pid1, NULL, 0);
    switch (pid2 = fork()) {
        case -1:
            exit(EXIT_FAILURE);
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

    switch (pid3 = fork()) {
        case -1:
            exit(EXIT_FAILURE);
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
    switch (pid4 = fork()) {
        case -1:
            exit(EXIT_FAILURE);
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