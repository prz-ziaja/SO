/*
** server.c -- a stream socket server demo
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
 
#define MYPORT 3030	// the port users will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define BACKLOG 10	 // how many pending connections queue will hold
 
void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
 
int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr;	// my address information
	struct sockaddr_in their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1, pid, numbytes;
	char buf[MAXDATASIZE];
 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
 
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
 
	my_addr.sin_family = AF_INET;		 // host byte order
	my_addr.sin_port = htons(MYPORT);	 // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);
 
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		perror("bind");
		exit(1);
	}
 
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
 
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
 
	sin_size = sizeof their_addr;

	if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) != -1) {
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {	
			while (buf[0] != EOF) {
				scanf("%s", buf);
				if (send(new_fd, buf, strlen(buf), 0) == -1) {
					perror("send");
					exit(EXIT_FAILURE);
				}
			}
			exit(EXIT_SUCCESS);
		}
		while (1) {
			if ((numbytes=recv(new_fd, buf, 10, 0)) == -1) {
				perror("recv");
				exit(1);
			}
			if (numbytes > 0) {
				if (buf[0] == EOF) 
					break;
				buf[numbytes] = '\0';
				printf("Client: %s \n", buf);
				fflush(stdout);
			}
			else if (numbytes < 0) {
				perror("read");
				fflush(stdout);
			}
		}
		close(sockfd);
		exit(EXIT_SUCCESS);	 
	}
	return 0;
}
