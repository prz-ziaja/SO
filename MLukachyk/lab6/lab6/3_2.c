
#include <stdio.h>   //size_t
#include <string.h>
#include <stdlib.h>   //exit
#include <unistd.h> //pipe



int main(int argc, char *argv[]){
    int pfd1[2], pfd2[2];
    char buff1[100], buff2[100], buff3[2], buff4[20];
    int pid1, pid2, put, i;
    size_t nread;

    if((pipe(pfd1) == -1) || (pipe(pfd2) == -1)){
        printf("\nError: Something went wrong!");
        exit(EXIT_FAILURE);
    }


    switch (pid1 = fork()){
        case -1 :
            exit(EXIT_FAILURE);
        case 0 :
            close(pfd1[0]);
            close(STDOUT_FILENO);
            dup2(pfd1[1], STDOUT_FILENO);
            execlp("seq", "seq", "10", NULL); //
            break;
        default:
            switch(pid2 = fork()) {
                case -1:
                    exit(EXIT_FAILURE);

                case 0:
                    close(pfd1[1]);
                    close(pfd2[0]);
                    read(pfd1[0], buff1, 100);
                    for(i = 0; i < 20; i += 2){
                        buff3[0] = buff1[i];
                        buff3[1] = buff1[i+1];
                        put = snprintf(buff4, 20, "%d\n", atoi(buff3) * 5);  // atoi - convert string to integer
                        if (i == 18) {
                            buff3[put++] = '\0';
                        }
                        write(pfd2[1], buff4, put);

                    }
                    close(pfd2[1]);
                    break;
                default:
                    close(pfd2[1]);
                    waitpid(pid1, NULL, 0);   //wait for a specific child process to end
                    waitpid(pid2, NULL, 0);
                    while (((nread = read(pfd2[0], buff2, 100)) > 0)){
                        write(STDOUT_FILENO, buff2, nread);
                    }
                    break;



            }

    }
}

