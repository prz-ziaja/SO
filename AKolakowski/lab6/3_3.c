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
	int     pfd[2], pfd2[2], pfd3[2], pfd4[2], pfd5[2];
	pid_t	pid;

	int nread;
	char buf[BUFSIZE];

	pipe(pfd);
	pid = fork();
	if(pid == 0) /* child 1 */
	{
		close(pfd[0]);
		dup2(pfd[1],1);
		execlp("who","who",NULL);
	}	
	close(pfd[1]);

	pipe(pfd2);
	pid = fork();
	if(pid == 0) /* child 2 */
	{
		close(pfd2[0]);
		dup2(pfd[0],0);
		dup2(pfd2[1],1);
		execlp("cut","cut","-d ","-f1",NULL);
	}
	close(pfd2[1]);

	pipe(pfd3);
	pid = fork();
	if(pid == 0) /* child 3 */
	{
		close(pfd3[0]);
		dup2(pfd2[0],0);
		dup2(pfd3[1],1);
		execlp("sort","sort",NULL);
	}
	close(pfd3[1]);

	pipe(pfd4);
	pid = fork();
	if(pid == 0) /* child 4 */
	{
		close(pfd4[0]);
		dup2(pfd3[0],0);
		dup2(pfd4[1],1);
		execlp("uniq","uniq","-c",NULL);
	}
	close(pfd4[1]);

	pipe(pfd5);
	pid = fork();
	if(pid == 0) /* child 5 */
	{
		close(pfd5[0]);
		dup2(pfd4[0],0);
		dup2(pfd5[1],1);
		execlp("sort","sort","-r",NULL);
	}
	close(pfd5[1]);

	nread = read(pfd5[0],buf,sizeof(buf));
	write(1,buf,nread);

	return 0;
}
