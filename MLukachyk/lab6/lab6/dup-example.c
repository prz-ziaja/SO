//
// Created by Maryna Lukachyk on 2020-04-21.
//

#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int     pfd[2];
    pid_t   pid;
    char    string[] = "Test";
    char    buf[10];

    pipe(pfd);


    if(pipe(pfd) == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }


    pid = fork();


    if(pid == 0) {
        close(STDIN_FILENO);
        dup2(pfd[0], STDIN_FILENO );
        close(pfd[1]);
        close(pfd[0]);
        read(STDIN_FILENO, buf, sizeof(buf));
        printf("Wypisuje: %s", buf);
    } else {
        close(pfd[0]);
        write(pfd[1], string, (strlen(string)+1));

    }

    return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

