#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
void backward(char *path)
{
	char s[256], c;
	int i, fd;
	off_t where;
	
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
	backward(argv[1]);
}
