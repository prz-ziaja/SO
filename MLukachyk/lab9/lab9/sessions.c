//
// Created by Maryna Lukachyk on 2020-05-14.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {

    pid_t pid, sid;
    pid = fork();
    sid = getsid(0);
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) {
        printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
    }
    if (pid == 0) {
        setsid();
        printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),getsid(0));
    }

    exit(EXIT_SUCCESS);
}


