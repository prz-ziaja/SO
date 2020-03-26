#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/times.h>
#include "t.h"
#define BUFSIZE 2048


static struct tms tbuf1;
static clock_t real1;
static long clock_ticks;

void timestart(void)
{
	real1 = times(&tbuf1);
	clock_ticks = sysconf(_SC_CLK_TCK);
	return;
}

void timestop(char *msg)
{
	struct tms tbuf2;
	clock_t real2;

	real2 = times(&tbuf2);
	fprintf(stdout, "%s:\n\t\"Total (user/sys/real)\", %ld, %ld, %ld\n\t\"Child (user/sys)\", %ld, %ld\n", 
		msg,
	  	(long)((tbuf2.tms_utime + tbuf2.tms_cutime) - (tbuf1.tms_utime + tbuf1.tms_cutime)),
	  	(long)((tbuf2.tms_stime + tbuf2.tms_cstime) - (tbuf1.tms_stime + tbuf1.tms_cstime)),
		(long)(real2 - real1),
	  	(long)(tbuf2.tms_cutime - tbuf1.tms_cutime),
	  	(long)(tbuf2.tms_cstime - tbuf1.tms_cstime));
	return;
}

void copy1(char *from, char *to)  /* has a bug */
{
	int fromfd = -1, tofd = -1;
	ssize_t nread;
	char buf[BUFSIZE];
	
	fromfd = open(from, O_RDONLY);
	tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR);
	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
	    write(tofd, buf, nread);	
	
        close(fromfd);
	close(tofd);
	return;
}

void copy2(char *from, char *to) // moj copy1 
{
	int fromfd = -1, tofd = -1;
	ssize_t nread;
	char buf[BUFSIZE];
	
	fromfd = open(from, O_RDONLY);
	if(fromfd==-1){
		if(errno==EACCES)
			perror("Error in file1: access denied");
		else if(errno==ENOENT)
			perror("Error in file1: file does not exist");
		else
			perror("Error in file1: unknow error");
		exit(1);		
	}
	tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
	if(tofd==-1){
		if(errno == EEXIST)
      			perror("Error in file2: file exists");
    		else
      			perror("Error in file2: unknow error");
		exit(1);	
	}

	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
	    	if(write(tofd, buf, nread) == -1){
			close(fromfd);
			close(tofd);
			perror("Error:write");
			exit(1);	
		}
	if(nread == -1){
		close(fromfd);
		close(tofd);
		perror("Error:read");
		exit(1);	
	}
        
	return;
}

void copy3(char *from, char *to)
{
	FILE *stfrom, *stto;
	int c;
	
	if ((stfrom = fopen(from, "r") ) == NULL){}
	if(( stto = fopen(to, "w") ) == NULL) {}
	while ((c = getc(stfrom)) != EOF)
		putc(c, stto);
	fclose(stfrom);
	fclose(stto);
	return;
	
}


int main(int argc, char **argv){
	if (argc != 3)
	{
		fprintf(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);
		return 1;
	}
	timestart();
	copy1(argv[1], argv[2]);
	timestop("copy1");

	timestart();
	copy3(argv[1], argv[2]);
	timestop("copy3");

	return 0;
}
