#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(void)
{
	int     pfd_r1[2], pfd_w1[2];
	pid_t	pid;

        if(pipe(pfd_r1) == -1 || pipe(pfd_w1) == -1)
	{
		perror("pipe");
		return 1;
	}

	/* creating first child */
	pid = fork();
        if(pid == -1)
	{
		perror("fork");
		return 1;
	}

	if(pid == 0) /* child 1 */
	{
		char *cmd, *argv[3];
		close(pfd_r1[0]);
		close(pfd_w1[0]);
		close(pfd_w1[1]);

		dup2(pfd_r1[1],1);

		cmd = "seq";
		argv[0] = "seq";
		argv[1] = "10";
		argv[2] = NULL;
		execvp(cmd,argv);

	}
	else /* parent */
	{
		/* creating second child */
		pid = fork();
		if(pid == -1)
		{
			perror("fork");
			return 1;
		}

		if(pid == 0) /* child 2 */
		{
			char buf[BUFSIZE];
			char *ch;
			int nread, num;
			
			close(pfd_r1[1]);
			close(pfd_w1[0]);

			while((nread=read(pfd_r1[0], &buf, BUFSIZE)) > 0)
			{
				ch = strtok(buf, "\n");
				while (ch != NULL)
				{
					num = atoi(ch)*5;
					ch = strtok(NULL, "\n");
					dprintf(pfd_w1[1],"%d\n",num);
				}
			}

			close(pfd_r1[0]);
			close(pfd_w1[1]);
		}
		else /* parent */
		{
			char buf[BUFSIZE];
			int nread;

			close(pfd_r1[0]);
			close(pfd_r1[1]);
			close(pfd_w1[1]);

			while((nread=read(pfd_w1[0], &buf, BUFSIZE)) > 0)
			{
				printf("%s",buf);
			}

			close(pfd_w1[0]);


		}

	}
	
	return 0;

}
