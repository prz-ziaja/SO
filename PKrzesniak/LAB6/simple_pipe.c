#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int pdesc[2];

    if (pipe(pdesc) == -1)
    {
        perror("Error creating unnamed pipe ");
        exit(1);
    }
    char buf[20];
    long buff_size = fpathconf(pdesc[0], _PC_PIPE_BUF);

    printf("Buff size: %ld\n", buff_size);

    switch (fork())
    {
    case -1:
        perror("error when forking process");
        exit(1);
        break;
    case 0:
        close(pdesc[1]);

        if (read(pdesc[0], buf, 20) == -1)
        {
            perror("error reading from pipe");
            exit(1);
        }
        printf("%s \n", buf);
        break;
    default:
        close(pdesc[0]);
        if (write(pdesc[1], "Hello world!Hello people", 20) == -1)
        {
            perror("errow writing to pipe");
            exit(1);
        }
        break;
    }
}
