#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    int status;
    int pid;
    int fd;

    pid = fork();

    if (pid == -1)
    {
        printf("Error: Start Daemon failed (%s)\n", strerror(errno));
        return -1;
    } else if (pid > 0){
        return 0;
    }
    else if (pid == 0)
    {
        umask(0);
        setsid();
        chdir("/");

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    fd = open("demon", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }

    if (dup2(fd, 1) == -1) {
        perror("dup2 failed");
        exit(1);
    }

    int y=0;
    while(y<60){
        system("shuf -i1-49 -n 6 > wyniki_lotto");
        y+=1;
        sleep(5);
    }
    close(STDOUT_FILENO);
    exit(0);
}

