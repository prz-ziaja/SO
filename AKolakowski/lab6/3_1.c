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
		char buf[BUFSIZE];
		int i, cnt=0, nread;
		
		close(pfd_r1[1]);
		close(pfd_w1[0]);

		while((nread=read(pfd_r1[0], &buf, BUFSIZE)) > 0)
		{	
			for(i=0;i<nread;i++)
			{	
				if(buf[i] == '\n')
					cnt++;
			}
		}

		close(pfd_r1[0]);

		dprintf(pfd_w1[1],"Number of lines: %d\n",cnt);
		close(pfd_w1[1]);

	}
	else /* parent */
	{
		int pfd_r2[2], pfd_w2[2];

		if(pipe(pfd_r2) == -1 || pipe(pfd_w2) == -1)
		{
			perror("pipe");
			return 1;
		}

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
			int i, prev, cnt=0, nread;
			
			/* closing child 1 pipes */
			close(pfd_r1[0]);
			close(pfd_r1[1]);
			close(pfd_w1[0]);
			close(pfd_w1[1]);

			close(pfd_r2[1]);
			close(pfd_w2[0]);

			while((nread=read(pfd_r2[0], &buf, BUFSIZE)) > 0)
			{
				prev=0;
	
				for(i=0;i<nread;i++)
				{	
					if(buf[i] == '\n')
					{
						if(strstr(buf+prev,"pipe") != NULL && strstr(buf+prev,"pipe") < &buf[i])
						{
						cnt++;
						}
						
						prev=i;
					}
						
				}
			}

			close(pfd_r2[0]);

			dprintf(pfd_w2[1],"Number of lines with \"pipe\": %d\n",cnt);			
			close(pfd_w2[1]);
		}
		else /* parent */
		{
			char buf[BUFSIZE];
			int dict_fd = open("dictionary.txt",O_RDONLY); 
			int nread;

			close(pfd_r1[0]);
			close(pfd_w1[1]);
			close(pfd_r2[0]);
			close(pfd_w2[1]);

			while((nread=read(dict_fd, &buf, BUFSIZE)) > 0)
			{
				write(pfd_r1[1],buf,nread);
				write(pfd_r2[1],buf,nread);
			}
			
			close(pfd_r1[1]);
			close(pfd_r2[1]);

			nread=read(pfd_w1[0], &buf, BUFSIZE);
			close(pfd_w1[0]);
			write(1,buf,nread);

			nread=read(pfd_w2[0], &buf, BUFSIZE);
			close(pfd_w2[0]);
			write(1,buf,nread);

		}

	}
	
	return 0;

}
