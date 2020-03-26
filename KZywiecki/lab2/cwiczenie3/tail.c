#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 512

void pushQueue(int* tab, int number)
{
    for(int i = 1; i < 10; i++)
    {
        tab[i - 1] = tab[i];
    }
    tab[9] = number;
}

void printLines(char* path)
{
    char buf[BUFSIZE];
    int fd = open(path, O_RDONLY);
    if(fd == -1)
    {
        fprintf(stderr, strerror(errno));
    }
    int positions[10];
    int count = 0;

    int attempt;
    int index = 0;
    do
    {
        attempt = read(fd, buf, BUFSIZE);
        for(int i = 0; i < attempt; i++)
        {
            if(buf[i] == '\n')
            {
                if(count < 10)
                {
                    positions[count] = index + i;
                    count++;
                }
                else
                {
                    pushQueue(positions, i + attempt);
                }
            }
        }
        index += attempt;
    } while (attempt != 0);
    if(count < 9)
    {
        lseek(fd, 0l, SEEK_SET);
    }
    else
    {
        lseek(fd, positions[0]+1, SEEK_SET);
    }
    do
    {
        memset(buf, 0, BUFSIZE);
        attempt = read(fd, buf, BUFSIZE - 1);
        printf(buf);
    } while (attempt != 0);
}

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        printLines(argv[1]);
    }
    else
    {
        printf("aaa");
    }
    
}