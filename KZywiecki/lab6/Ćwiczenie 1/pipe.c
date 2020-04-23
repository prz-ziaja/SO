#include <stdio.h>
#include <unistd.h>
#include <string.h>
    
int main(int argc, char *argv[]){
    
    int pfd[2];
    size_t nread;
    char buf[100];

    pipe(pfd);

    long size = fpathconf(pfd[0], _PC_PIPE_BUF);
    printf("Max bytes: %ld\n", size);
    int res = fork();
    switch (res)
    {
    case -1:
        printf("Błąd wystąpił przy fork()");
        break;
    case 0:
        close(pfd[1]);
        nread = read(pfd[0], buf, 100);
        (nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
        break;
    default:
        close(pfd[0]);
        char* str2 = "Witam sasiada";
        write(pfd[1], str2, strlen(str2));
        break;
    }


    return 0;
}

