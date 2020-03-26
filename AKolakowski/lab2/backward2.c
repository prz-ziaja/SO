#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

void backward(char *path)
{
	char s[256], c;
	int i, fd;
	off_t where;
	
	fd = open(path, O_RDONLY);
	where = lseek(fd, 0, SEEK_END);
	i = sizeof(s) - 1;
	s[i] = '\0';
	do {
		switch (pread(fd, &c, 1, --where)) {
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

int main(int argc, char **argv)
{
	if(argc!=2)
		return 1;

	backward(argv[1]);

	return 0;
}
