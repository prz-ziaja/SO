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
	int sockfd, pid;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	socklen_t addr_len;
	int numbytes;
	char buf[MAXBUFLEN];

	if (argc != 5) {
		fprintf(stderr,"usage: udpchat my_addr my_port thier_addr thier_port\n");
		exit(EXIT_FAILURE);
	}

	my_addr.sin_family = AF_INET;		 
	my_addr.sin_port = htons(atoi(argv[2]));
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(atoi(argv[4]));
	their_addr.sin_addr.s_addr = inet_addr(argv[3]);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	addr_len = sizeof their_addr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Error: socket");
		exit(EXIT_FAILURE);
	}

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		perror("Error: bind");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if(pid == -1){
		perror("Error: fork");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		while(1){
			if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
				perror("Error: recvfrom");
				exit(EXIT_FAILURE);
			}
			buf[numbytes] = '\0';
			printf("got packet from %s",inet_ntoa(their_addr.sin_addr));
			printf("packet is %d bytes long\n",numbytes);
			printf("packet contains \"%s\" \n",buf);

			if(strcmp(buf,"bye") == 0){
				kill(pid,SIGTERM);
				close(sockfd);
                		exit(EXIT_SUCCESS);
			}	
		}
	}

	else{
		while(1){
			if (fgets(buf, MAXBUFLEN, stdin) == NULL) {
				perror("Error: fgets");
				exit(EXIT_FAILURE);
			    }
			if ((sendto(sockfd, buf, strlen(buf)-1, 0, (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
				perror("Error: sendto");
				exit(EXIT_FAILURE);
            		}
			if(strcmp(buf,"bye\n") == 0){
				kill(pid,SIGTERM);
				close(sockfd);
                		exit(EXIT_SUCCESS);
			}	
		}
	}
	return 0;
}
