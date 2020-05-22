#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>

#define PORT 80
#define MAXDATASIZE 10000

int main(void)
{
    char url[100], s[500], *p;
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in addr;
 
    while (1) {
		printf("URL: ");
		if (fgets(url, sizeof(url), stdin) == NULL)
		  break;
		if ((p = strrchr(url, '\n')) != NULL)
		  *p = '\0';
		if ((p = strchr(url, '/')) != NULL) {
		  *p = '\0';
		}
		snprintf(s, sizeof(s), "//%s:80", url);

		if ((he=gethostbyname(url)) == NULL) {
		    perror("gethostbyname");
		    exit(1);
		}
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		    perror("socket");
		    exit(1);
		}
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr = *((struct in_addr *)he->h_addr);
		memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
		if (connect(sockfd, (struct sockaddr *)&addr, sizeof addr) == -1) {
		    perror("connect");
		    exit(1);
		}

		snprintf(s, sizeof(s), "GET / HTTP/1.0\r\n\r\n");

		if (send(sockfd, s, sizeof(s), 0) == -1){
		    perror("send");
		    exit(1);
		}
		if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
		    perror("recv");
		    exit(1);
		}
		buf[numbytes] = '\0';
		printf("%s\n",buf);
		close(sockfd);
	}
 }
