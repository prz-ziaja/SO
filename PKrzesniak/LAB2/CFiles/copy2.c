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

void copy(char *from, char *to)  
{
	int fromfd = -1, tofd = -1;
	ssize_t nread;
	char buf[BUFSIZE];

	fromfd = open(from, O_RDONLY);
	if(fromfd ==-1){
		if(errno == EACCES){          
			perror("ERROR: Brak dostępu");
		} else if(errno == ENOENT){   
			perror("ERROR: Plik nie istnieje!");
		} else {
			perror("ERROR");
			exit(EXIT_FAILURE);
		}
	}
	tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR);
	if(tofd ==-1){
		if(errno == EACCES){          
			perror("ERROR: Brak dostępu");
		} else if(errno == ENOENT){   
			perror("ERROR: Plik nie istnieje!");
		} else {
			perror("ERROR");
			exit(EXIT_FAILURE);
		}
	}

	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
		write(tofd, buf, nread);	

	close(fromfd);
	close(tofd);
	return;
}

int main(int argc, char **argv){
	if(argc < 3){
		printf("Please enter source and target file!");
		exit(EXIT_FAILURE);
	}

	copy(argv[1], argv[2]);

	return 0;
}
