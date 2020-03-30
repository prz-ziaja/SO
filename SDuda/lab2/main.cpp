#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024

void cat(char **files, int ifNum, int argc) 
{
	int fromfd = -1, j;
	ssize_t nread, nwrite;
	char buf[BUFSIZE];
	char* file;
	if(ifNum)
		j=2;
	else
		j=1;
	while(j<argc){
		file=files[j];
		fprintf(stdout,"%s\n",file);
		fromfd = open(file, O_RDONLY);
		if(fromfd==-1){
			if(errno==EACCES)
				perror("Error f1: file rights");
			else if(errno==ENOENT)
				perror("Error f1: file does not exist");
			else
				perror("Error f1: other exception");
			exit(1);
		}
		int i=0;
		if(ifNum){
			fprintf(stdout,"%5.5d:   ",i=i+1);
			while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
				for(int k=0; k<nread; k=k+1){
					fprintf(stdout,"%c",buf[k]);
					if(buf[k]=='\n')
						fprintf(stdout,"%5.5d:   ",i=i+1);
				}
		}
		else
			while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
				for(int k=0; k<nread; k=k+1)
					fprintf(stdout,"%c",buf[k]);

		close(fromfd);
		if(nread==-1){
			perror("Read");	
			exit(1);
		}
		fromfd=-1;
		j+=1;
		fprintf(stdout,"\n\n");
	}
	return;
}

int main(int argc, char **argv){
	if (argc < 2)
	{
		fprintf(stderr,"usage of cat: [-n] path\n");
		return 1;
	}
	if (strcmp(argv[1],"-n")==0)
		cat(argv,1,argc);
	else
		cat(argv,0,argc);
	return 0;
}