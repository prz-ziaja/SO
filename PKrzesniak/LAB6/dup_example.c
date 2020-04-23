#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
        int     pfd[2];
        pid_t   pid;
        char    string[] = "Test";
        char    buf[10];

       if (pipe(pfd) == -1) {
        perror("Pipe error! ");
        exit(EXIT_FAILURE);
    }
        pid = fork();

        if(pid == 0) { 
		close(STDIN_FILENO);	
 		dup2(pfd[0],0); 
		close(pfd[1]);			
        	read(STDIN_FILENO, buf, sizeof(buf));
                printf("Wypisuje: %s", buf);
		close(pfd[0]);
        } else { 
		close(pfd[0]);
        	write(pfd[1], string, (strlen(string)+1));   
		close(pfd[1]);     
	}

        return 0;
}
