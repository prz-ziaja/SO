#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
void backward(char **fpath, int ifNum, int argc)
{
	char *path,s[256], c,nread,j;
	int i, fd,linenum;
	off_t where;
	if(ifNum){
		j=3;
		linenum=atoi(fpath[2]);
	}
	else{
		j=1;
		linenum=5;
	}
	while(j<argc){
		path=fpath[j];
		fprintf(stdout,"%s\n",path);
		fd = open(path, O_RDONLY);
		if(fd==-1){
			close(fd);
			perror("File problem, not able to open");
			exit(1);	
		}
		where = lseek(fd, 1, SEEK_END);
		i = sizeof(s) - 1;
		s[i] = '\0';
		do {
			where = lseek(fd, -2, SEEK_CUR);
			switch (read(fd, &c, 1)) {
				case 1:
					if (c == '\n') {
						linenum-=1;
						i = sizeof(s) - 1;
					}
					if (i <= 0) {
						errno = E2BIG;
						/* handle the error */
					}
					s[--i] = c;
					break;
				case -1:
					/* handle the error */
					break;
				default: /* impossible */
					errno = 0;
					/* handle the error */
			}
		} while (where > 0&&linenum>0);
		while ((nread = read(fd, s, sizeof(s))) > 0)
			for(int k=0; k<nread; k=k+1)
				fprintf(stdout,"%c",s[k]);
		close(fd);
		j+=1;
	}
	return;
}
int main(int argc, char** argv){
	if (argc < 2)
	{
		fprintf(stderr,"usage of tail: [-n number] path\n");
		return 1;
	}
	if (strcmp(argv[1],"-n")==0)
		backward(argv,1,argc);
	else
		backward(argv,0,argc);
	return 0;
}