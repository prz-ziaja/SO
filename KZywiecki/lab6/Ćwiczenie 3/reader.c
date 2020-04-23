#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stdlib.h"

int inputFile;

int lineInputpfd[2];
int lineOutputpfd[2];
int pipeInputpfd[2];
int pipeOutputpfd[2];

#define BUFSIZE 1024

char buffer[BUFSIZE];
char* line;

int pid2;
int readSize;
int count;

int findSubPipe(char* str)
{
    char pipe[] = "pipe";
    char* result = strstr(pipe, str);
    if(result == NULL)
    {
        return 0;
    }
    return 1;
}

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        printf("Nie podano nazwy pliku");
        return -1;
    }
    inputFile = open(argv[1], O_RDONLY);

    if(pipe(lineInputpfd) || pipe(lineOutputpfd) || pipe(pipeInputpfd) || pipe(pipeOutputpfd))
	{
		printf("AAAAAAA");
	}


    int pid1 = fork();
    switch(pid1)
    {
        case -1:
            return -2;
        break;
        case 0:
            close(STDIN_FILENO);
            dup2(lineInputpfd[0], 0);
            close(lineInputpfd[0]);
            close(lineInputpfd[1]);
            close(lineOutputpfd[0]);
            close(pipeInputpfd[1]);
            count = 0;
            readSize = 0;
            while((readSize = getline(&line, NULL, stdin)) != -1)
            {
                ++count;
            }
            write(lineOutputpfd[1], &count, sizeof(int));
            close(lineOutputpfd[1]);
            return 0;
        break;
        default:
            pid2 = fork();
            switch(pid2)
            {
                case -1:
                    return -2;
                break;
                case 0:
                    close(STDIN_FILENO);
                    dup2(pipeInputpfd[0], 0);
                    close(pipeInputpfd[0]);
                    close(pipeInputpfd[1]);
                    close(pipeOutputpfd[0]);
                    close(lineInputpfd[1]);
                    count = 0;
                    readSize = 0;
                    while((readSize = getline(&line, NULL, stdin)) != -1)
                    {
                        if(findSubPipe(line))
                        {
                            ++count;
                        }
                    }
                    write(pipeOutputpfd[1], count, sizeof(int));
                    close(pipeInputpfd[1]);
                    return 0;
                break;
                default:
                    readSize = 0;
                    while((readSize = read(inputFile, buffer, BUFSIZE)) != 0)
                    {
                        write(lineInputpfd[1], buffer, readSize);
                        write(pipeInputpfd[1], buffer, readSize);
                    }

                    int lCount = 0;
                    int pCount = 0;
                    read(lineOutputpfd[0], lCount, sizeof(int));
                    read(pipeOutputpfd[0], pCount, sizeof(int));
                
                    printf("%d, %d", lCount, pCount);
                break;
            }
        break;
    }

}
