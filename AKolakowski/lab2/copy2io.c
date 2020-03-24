#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include "t.h"

#define BUFSIZE 512

void copy(char *from, char *to)
{
	int fromfd = -1, tofd = -1;
	ssize_t nread, nwrite;
	char buf[BUFSIZE];
	
	fromfd = open(from, O_RDONLY);
	if(fromfd == -1)
	{
		if(errno == ENOENT)
			fprintf(stderr,"Plik nie istnieje\n");
		if(errno == EACCES)
			fprintf(stderr,"Brak dostepu\n");

		exit(EXIT_FAILURE);
	}

	tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR);
	if(tofd == -1)
	{
		if(errno == EACCES)
			fprintf(stderr,"Brak dostepu\n");

		exit(EXIT_FAILURE);			
	}

	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
	{
		if(nread == -1)
		{
			fprintf(stderr,"Blad odczytu\n");
			exit(EXIT_FAILURE);			
		}		

		nwrite = write(tofd, buf, nread);
	
		if(nwrite == -1)
		{
			fprintf(stderr,"Blad zapisu\n");
			exit(EXIT_FAILURE);			
		}		
	}
	    
	
    close(fromfd);
	close(tofd);
	return;
}

int main(int argc, char **argv){
	if (argc != 3)
	{
		fprintf(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);
		return 1;
	}

	timestart();
	copy(argv[1], argv[2]);
	timestop(NULL);

	return 0;
}
