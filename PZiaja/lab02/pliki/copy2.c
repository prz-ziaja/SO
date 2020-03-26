/*
 * copy1.c
 * 
 * based on: 
 *
 * Miscellaneous examples for Chap. 2
 * AUP2, Chap. 2
 *
 * modified by WTA
 */

/*[copy-with-bug]*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#define BUFSIZE 512

void copy(char *from, char *to)  /* has a bug */
{
	int fromfd = -1, tofd = -1;
	ssize_t nread, nwrite;
	char buf[BUFSIZE];
	fromfd = open(from, O_RDONLY);
	if(fromfd==-1){
		if(errno==EACCES)
			perror("Error f1: file rights");
		else if(errno==ENOENT)
			perror("Error f1: file does not exist");
		else
			perror("Error f1: other exception");
		exit(1);
	}
	tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR);
	if(tofd==-1){
		if(errno==EACCES)
			perror("Error f2: file rights");
		else if(errno==EEXIST)
			perror("Error f2: file exist");
		else
			perror("Error f2: other exception");
		exit(1);
	}
	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
	    if(nwrite=write(tofd, buf, nread)==-1)
		break;
        close(fromfd);
	close(tofd);
	if(nwrite=-1){
		perror("Write");
		exit(1);
	}
	if(nread==-1){
		perror("Read");	
		exit(1);
	}
	return;
}

int main(int argc, char **argv){
	if (argc != 3)
	{
		fprintf(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);
		return 1;
	}
	copy(argv[1], argv[2]);
	return 0;
}
