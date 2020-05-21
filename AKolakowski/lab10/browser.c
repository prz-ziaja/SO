#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(void)
{
    char url[100], s[500], *path = "", *p;
    int fd;
    ssize_t nread;

	struct hostent *he;
	struct sockaddr_in their_addr;
  
    while (1) {
          printf("URL: ");
          if (fgets(url, sizeof(url), stdin) == NULL)
              break;
          if ((p = strrchr(url, '\n')) != NULL)
              *p = '\0';
          if ((p = strchr(url, '/')) != NULL) {
              path = p + 1;
              *p = '\0';
          }
          snprintf(s, sizeof(s), "%s", url);
          
	if ((he=gethostbyname(s)) == NULL)
	{
		herror("gethostbyname");
		exit(1);
	}

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    perror("socket");
	    exit(1);
	}

	their_addr.sin_family = AF_INET;    // host byte order 
	their_addr.sin_port = htons(80);  // short, network byte order 
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	if (connect(fd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1) {
	    perror("connect");
	    exit(1);
	}

    snprintf(s, sizeof(s), "GET /%s HTTP/1.0\r\n\r\n", path);
	
	if (send(fd, s, strlen(s), 0) == -1)
	{
		perror("send");
		exit(1);
	}

	do
	{
		switch(nread = recv(fd, s, sizeof(s), 0))
		{
			case 0:
				close(fd);
				break;
			case -1:
				perror("recv");
				exit(1);
			default:
				write(STDOUT_FILENO, s, nread);
		}
	}
	while(nread);

    }
 }
