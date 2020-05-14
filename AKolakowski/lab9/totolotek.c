#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	pid_t pid;

	pid = fork();

	switch(pid)
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:	/* dziecko */
			umask(0);

			if(setsid() == -1)
			{
				perror("setsid");
				exit(EXIT_FAILURE);
			}

			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			
			while(1)
			{
				system("shuf -i1-49 -n 6 > wyniki_lotto");
				sleep(20);
			}

			exit(EXIT_SUCCESS);
		default: /* rodzic */
			exit(EXIT_SUCCESS); 
	}

}
