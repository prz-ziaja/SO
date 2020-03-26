#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFSIZE 512

void copy(char *from, char *to)
{
	int fromfd = -1, tofd = -1;
	ssize_t nread;
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
	    write(tofd, buf, nread);	
	
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
	copy(argv[1], argv[2]);
	return 0;
}