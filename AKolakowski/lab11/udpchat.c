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

#define MAXBUFLEN 100

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr_r, their_addr_w;
	socklen_t addr_len;
	int numbytes;
	char buf[MAXBUFLEN];
	pid_t pid;

	if (argc != 5) {
		fprintf(stderr,"usage: udpchat RECEIVE_ADDR RECEIVE_PORT SEND_ADDR SEND_PORT\n");
		exit(1);
	}

	my_addr.sin_family = AF_INET;		 
	my_addr.sin_port = htons(atoi(argv[2]));
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	their_addr_w.sin_family = AF_INET;
	their_addr_w.sin_port = htons(atoi(argv[4]));
	their_addr_w.sin_addr.s_addr = inet_addr(argv[3]);
	memset(their_addr_w.sin_zero, '\0', sizeof their_addr_w.sin_zero);

	addr_len = sizeof their_addr_r;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		perror("bind");
		exit(1);
	}

	switch(pid = fork())
	{
		case -1: /* error */
			perror("fork");
			close(sockfd);
			exit(1);
		case 0: /* writing to other end */
			while(1)
			{
				if((numbytes = read(STDIN_FILENO,buf,MAXBUFLEN)) == -1)
				{
					perror("read");
					exit(1);
				}

				if ((numbytes = sendto(sockfd, buf, numbytes-1, 0, (struct sockaddr *)&their_addr_w, sizeof their_addr_w)) == -1)
				{
					perror("sendto");
					exit(1);
				}
			}
		default: /* reading from other end */
			while(1)
			{
				if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr_r, &addr_len)) == -1)
				{
					perror("recvfrom");
					exit(1);
				}
				
				buf[numbytes] = '\0';

				printf("got %d bytes packet from %s: \"%s\"\n",numbytes,inet_ntoa(their_addr_r.sin_addr),buf);

				if(strcmp(buf,"bye") == 0)
				{
					if(kill(pid,SIGTERM) == -1)
					{
						perror("kill");
						exit(1);
					}
					break;
				}	
			}
	}

	close(sockfd);

	return 0;
}
