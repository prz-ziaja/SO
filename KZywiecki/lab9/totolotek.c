#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

pid_t pid, sid;
char buffer[3];
int count;

void updateBuffer(int number)
{
    
    int index = 0;
    memset(buffer, 0, 3);
    while(number != 0)
    {
        buffer[index] = number%10 + '0';
        number /= 10;
        index++;
    }
    count = index;
    index--;
    for(int i = 0; i <= index; i++)
    {
        char temp = buffer[index];
        buffer[index] = buffer[index - i];
        buffer[index - i] = temp;
    }
}

int main() 
{
    srand(time(NULL));

    pid = fork();
    sid = getsid(0);
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) 
    {
        exit(EXIT_SUCCESS);
    }
    if (pid == 0) 
    {
        setsid();
        printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),getsid(0));
        int fd;
        fd = open("wynik", (O_WRONLY | O_CREAT) & ~O_APPEND, 0666);
        if (fd == -1) 
        {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < 6; i++)
        {
            updateBuffer(rand() % 49 + 1);
            write(fd, buffer, count);
            write(fd, "\n", 1);
            sleep(1);
        }
        close(fd);
    }
    exit(EXIT_SUCCESS);
}
