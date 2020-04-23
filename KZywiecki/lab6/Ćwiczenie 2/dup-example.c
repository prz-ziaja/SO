#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
    
int main(void)
{
    int     pfd[2];
    pid_t   pid;
    char    string[] = "Test";
    char    buf[10];

    pipe(pfd);
    pid = fork();

    if(pid == -1)
    {
        printf(strerror(errno));
    }
    else if(pid == 0) 
    {
    close(pfd[1]);
    close(0);              
    dup2(pfd[0], 0);
    close(pfd[0]);
    read(STDIN_FILENO, buf, sizeof(buf));
    printf("Wypisuje: %s", buf);
    } 
    else 
    {
        close(pfd[0]);
        write(pfd[1], string, (strlen(string)+1));        
    }
    
    return 0;
}

