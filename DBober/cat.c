#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024

void cat(char **files, int O, int argc) 
{
	int fromfd = -1, num=O+1;
	ssize_t nread, nwrite;
	char buf[BUFSIZE];
	char* file;
	while(num<argc){
		file=files[num];
		fromfd = open(file, O_RDONLY);
		if(fromfd == -1){
			if(errno==EACCES)
				perror("Error in file1: access denied");
			else if(errno==ENOENT)
				perror("Error in file1: file does not exist");
			else
				perror("Error in file1: unknow error");
			exit(1);		
		}
		while ((nread = read(fromfd, buf, sizeof(buf))) > 0){
		    	for(int i=0; i<nread; i++){
				fprintf(stdout,"%c",buf[i]);
				if (O && buf[i+1] == '\n')
					fprintf(stdout,"%c",'$');
			}
		}	

		close(fromfd);
		if(nread==-1){
			close(fromfd);
			perror("Error:read");
			exit(1)	;
		}
		fromfd=-1;
		num++;
	}
	return;
}

int main(int argc, char **argv){
	if (argc < 2)
	{
		fprintf(stderr,"usage of cat: [-n] path\n");
		return 1;
	}
	if (strcmp(argv[1],"-E") == 0)
		cat(argv,1,argc);
	else
		cat(argv,0,argc);
	return 0;
}
