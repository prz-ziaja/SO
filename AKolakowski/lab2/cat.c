#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFSIZE 512
enum {
    NUMBERS = 1 << 0, /* binary 01, numerowanie linii */
    DOLLARS = 1 << 1 /* binary 10, dolar na koncu linii */
};

int nline=1;

void copyToStdout(char *from, int flags)
{
	int fromfd = -1, tofd = -1;
	ssize_t nread, nwrite;
	char buf[BUFSIZE];
	
	char buf2[4*BUFSIZE];
	int i_buf2=0;
	
	if(from == NULL)
		fromfd = 0;
	else
	{
		fromfd = open(from, O_RDONLY);

		if(fromfd == -1)
		{
			if(errno == ENOENT)
				fprintf(stderr,"Plik nie istnieje\n");
			if(errno == EACCES)
				fprintf(stderr,"Brak dostepu\n");

			exit(EXIT_FAILURE);
		}
	}	


	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
	{
		if(nread == -1)
		{
			fprintf(stderr,"Blad odczytu\n");
			exit(EXIT_FAILURE);			
		}		

		if(flags & NUMBERS)/* wypisuje nr linii */
		{			
			char line[BUFSIZE];
			int n_line;
			
			int j, j2;

			i_buf2=0;

			n_line=sprintf(line,
			"     %d  ",nline++);			

			for(j2=0;j2<n_line;j2++)
				buf2[i_buf2++]=line[j2];

			for(j=0; j<nread; j++)
			{
				buf2[i_buf2++]=buf[j];

				if(buf[j]=='\n' && j < nread - 1)
				{
					n_line=sprintf(line,
					"     %d  ",nline++);

					for(j2=0;j2<n_line;j2++)
						buf2[i_buf2++]=line[j2];
				}
			}
		}
		else
		{
			int i;
			for(i=0;i<nread;i++)
				buf2[i]=buf[i];

			i_buf2=nread;
		}

		if(flags & DOLLARS)/* wypisuje dolar na koncu linii */
		{			
			int j, j2;

			for(j=0; j<i_buf2; j++)
			{
				if(buf2[j]=='\n')
				{
					for(j2=i_buf2;j2>j;j2--)
					{
						buf2[j2]=buf2[j2-1];
					}

					buf2[j]='$';
					i_buf2++;
					j++;
				}
			}
		}

		if(flags)
			nwrite = write(1, buf2, i_buf2);
		else
			nwrite = write(1, buf, nread);
	
		if(nwrite == -1)
		{
			fprintf(stderr,"Blad zapisu\n");
			exit(EXIT_FAILURE);			
		}		
	}
	
    close(fromfd);
	close(tofd);
	return;
}


int main(int argc, char **argv)
{
	int i, j, options=0, flags=0;

	for(i=1;i<argc;i++)/* zbiera opcje */
	{
		if(argv[i][0]=='-')
		{
			for(j=1;j<strlen(argv[i]);j++)
			{
				if(argv[i][j]=='n')
					flags=flags|NUMBERS;

				if(argv[i][j]=='E')
					flags=flags|DOLLARS;
			}

			options++;
		}
			
	}

	for(i=1;i<argc;i++)/* wlasciwe wywolanie cat */
	{
		if(argv[i][0]!='-')
			copyToStdout(argv[i], flags);
	}
	
	if(options+1==argc)
		copyToStdout(NULL, flags);

	return 0;
}
