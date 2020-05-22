#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
    
pid_t pid, sid;

int main(void) {
    pid = fork();
    sid = getsid(0);

    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) 
    {
        printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
    }
    if (pid == 0) 
    {
        sid = setsid();
        printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
    }
    exit(EXIT_SUCCESS);
}

