#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#define SERVER_PORT 5555
#define MAXBUFLEN 1000

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *he;
	int numbytes;
	char buf[MAXBUFLEN];
	char regtext[MAXBUFLEN];

	if (argc != 3) {
		fprintf(stderr,"usage: kalendarz_klient hostname username\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

	strcpy(regtext,"REGISTER");
	strcat(regtext,argv[2]);
	

	/* register user on server */
	if ((numbytes = sendto(sockfd, regtext, strlen(regtext), 0, (struct sockaddr *)&server_addr, sizeof server_addr)) == -1)
	{
		perror("sendto");
		exit(1);
	}

	switch(fork())
	{
		case -1: /* error */
			perror("fork");
			close(sockfd);
			exit(1);
		case 0: /* sending data to server */
			while(1)
			{
				if((numbytes = read(STDIN_FILENO,buf,MAXBUFLEN)) == -1)
				{
					perror("read");
					exit(1);
				}

				if ((numbytes = sendto(sockfd, buf, numbytes-1, 0, (struct sockaddr *)&server_addr, sizeof server_addr)) == -1)
				{
					perror("sendto");
					exit(1);
				}

			}
		default: /* receiving data from server */
			while(1)
			{
				if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, NULL, NULL)) == -1)
				{
					perror("recvfrom");
					exit(1);
				}
				
				buf[numbytes] = '\0';

				printf("got %d bytes packet: \n%s\n",numbytes,buf);
			}
	}

	close(sockfd);

	return 0;
}
