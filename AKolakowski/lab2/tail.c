#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

off_t findStart(char *path)
{
	char c;
	int fd;
	off_t where;
	int counter=0;
	
	fd = open(path, O_RDONLY);
	where = lseek(fd, 0, SEEK_END);

	do {
		switch (pread(fd, &c, 1, --where)) {
			case 1:
				if (c == '\n')
					counter++;
				break;
			case -1:
				/* handle the error */
				break;
		}

	} while (where > 0 && counter <= 10);

	close(fd);
	return where;
}


void readTail(char *path)
{
	char s[256];
	int r, fd;
	off_t where;
	
	where = findStart(path)+1;
	fd = open(path, O_RDONLY);

	while((r=pread(fd, s, sizeof(s), where+=r)))
		write(1,s,r);

	close(fd);
	return;
}

int main(int argc, char **argv)
{
	if(argc!=2)
		return 1;

	readTail(argv[1]);

	return 0;
}
