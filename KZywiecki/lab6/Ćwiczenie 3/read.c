#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 1024

char buffer[BUFSIZE];
char* line;
int bufsize = BUFSIZE;

int pfd1[2];
int pfd2[2];
int pfd1b[2];
int pfd2b[2];

int inputFile;
int nread;
int tread;
int counter = 0;

int pid1; 
int pid2;

int main(int argc, char** argv) 
{
    if(argc != 2)
    {
        printf("Nie podano nazwy pliku");
        return -1;
    }
    inputFile = open(argv[1], O_RDONLY);
    if(inputFile == -1)
    {
        printf("Blad pliku");
        return -1;
    }

    line = malloc(BUFSIZE);
    if ((pipe(pfd1) == -1) || (pipe(pfd2) == -1) || (pipe(pfd1b) == -1) || (pipe(pfd2b) == -1)) 
    {
        printf("Blad przy tworzeniu pipe");
        return -1;
    }

    pid1 = fork();
    switch(pid1)
    {
        case -1:
            return -1;
        break;
        case 0:
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            dup2(pfd1[0], STDIN_FILENO);
            dup2(pfd1b[1], STDOUT_FILENO);
            close(pfd1[1]);
            close(pfd1b[0]);
            while((tread = getline(&buffer, &bufsize, stdin)) != -1)
            {
                counter ++;
            }
            printf("%d", counter);
            return 0;
        break;
        default:
            pid2 = fork();
            switch(pid2)
            {
                case -1:
                return -1;
                break;
                case 0:
                    close(STDIN_FILENO);
                    close(STDOUT_FILENO);
                    dup2(pfd2[0], STDIN_FILENO);
                    dup2(pfd2b[1], STDOUT_FILENO);
                    close(pfd1[1]);
                    close(pfd1b[0]);
                    close(pfd2[1]);
                    close(pfd2b[0]);
                    while((tread = getline(&line, &bufsize, stdin)) != -1)
                    {
                        if(strstr(line, "pipe") != NULL)
                        {
                            counter ++;
                        }
                    }
                    printf("%d", counter);
                    return 0;
                break;
                default:
                    close(pfd1[0]);
                    close(pfd2[0]);
                    while ((nread = read(inputFile, buffer, BUFSIZE)) > 0) 
                    {
                        if (write(pfd1[1], buffer, nread) == -1) 
                        {
                            return -2;
                        };
                        if (write(pfd2[1], buffer, nread) == -1) 
                        {
                            return -2;
                        };
                    }

                    close(pfd1[1]);
                    close(pfd2[1]);
                    close(pfd1b[1]);
                    close(pfd2b[1]);

                    read(pfd1b[0], buffer, BUFSIZE);
                    printf("Liczba linii: %d \n", atoi(buffer));
                    read(pfd2b[0], buffer, BUFSIZE);
                    printf("Liczba linii z pipe: %d \n", atoi(buffer));
                break;
            }
        break;
    }
    free(line);
}