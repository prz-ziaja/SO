
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void backward(char *path)
{
	char s[256], c;
	int i, fd;
	off_t where;
	
	fd = open(path, O_RDONLY);
	if(fd==-1){
		close(fd);
		perror("Error");
		exit(1);
	}	
		

	//where = lseek(fd, 1, SEEK_END);

	i = sizeof(s) - 1;
	s[i] = '\0';
	do {
		where = lseek(fd, -2, SEEK_CUR);
		//switch (read(fd, &c, 1)) {
		switch(where = pread(fd, &c, 1, 2)){
			case 1:
				if (c == '\n') {
					printf("%s", &s[i]);
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
	} while (where > 0);
	printf("%s", &s[i]);
	close(fd);
	return;
}

int main(int argc, char** argv){
	if(argc != 2){
    		fprintf(stderr, "usage: %s path\n", argv[0]);
    		return -1;
  	}
 	else
    		backward(argv[1]);


  return 0;

}
