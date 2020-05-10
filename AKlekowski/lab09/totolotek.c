#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <time.h>

int main(void) {
    FILE *fp = NULL;
    pid_t pid, sid;
    pid = fork();
    sid = getsid(0);
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) {
        printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
        exit(EXIT_SUCCESS);
    }
    if (pid == 0) {
        printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
        umask(0);
        sid = setsid();

        if (sid < 0) exit(EXIT_FAILURE);

    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    fp = fopen ("wyniki_lotto.txt", "w+");
    time_t tt;
    srand(time(&tt));
    while (1)
    {
        sleep(5);
        fprintf(fp, "%d\n", (rand()%50)+1);
        fflush(fp);
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}
