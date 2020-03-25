#include "t.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>



void copy(char *from, char *to, int buffsize) 
{
	int fromfd = -1, tofd = -1;
	ssize_t nread;
	char buf[buffsize];

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

	while ((nread = read(fromfd, buf, buffsize)) > 0)
		write(tofd, buf, nread);	

	close(fromfd);
	close(tofd);
	return;
}

 
int main(int argc, char *argv[]){

  if (argc != 3)
	{
		fprintf(stderr,"Need 2 files! Copy from one file to another");
		return 1;
	}
    timestart();
    copy(argv[1], argv[2],1);
    timestop("Copy");

    timestart();
    copy(argv[1], argv[2],512);
    timestop("Copy");

    timestart();
    copy(argv[1], argv[2],1024);
    timestop("Copy");
 
    timestart();
    copy(argv[1], argv[2],1100);
    timestop("Copy");
	return 0;

}
