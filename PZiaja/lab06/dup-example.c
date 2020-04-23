#include <unistd.h>
#include <sys/types.h>

int main(void)
{
        int     pfd[2];
        pid_t   pid;
        char    string[] = "Test";
        char    buf[10];

	if (pipe(pfd) == -1) {
		perror("Blad pipe:");
		exit(0);
	}
        pid = fork();

        if(pid == 0) {
		close(STDIN_FILENO);  //zamkniecie stdin            
 		dup2(pfd[0], STDIN_FILENO);//przerobienie pipe'a na stdin
		close(pfd[0]);
		close(pfd[1]);
        	read(STDIN_FILENO, buf, sizeof(buf));
                printf("Wypisuje: %s\n", buf);
        } else {
		close(pfd[0]);
        	write(pfd[1], string, (strlen(string)+1));        
	}

        return 0;
}
