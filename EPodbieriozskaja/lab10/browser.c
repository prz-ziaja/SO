#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 80 // the port client will be connecting to

#define MAXDATASIZE 1000

int main(void)
{
    char url[100], s[500], *path = "", *p;
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
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
        snprintf(s, sizeof(s), "//%s:80", url);

        if ((he=gethostbyname(url)) == NULL) {  // get the host info
            perror("gethostbyname");
            exit(1);
        }

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        their_addr.sin_family = AF_INET;    // host byte order
        their_addr.sin_port = htons(PORT);  // short, network byte order
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

        if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1) {
            perror("connect");
            exit(1);
        }

        snprintf(s, sizeof(s), "GET /%s HTTP/1.0\r\n\r\n", path);

        if (send(sockfd, s, sizeof(s), 0) == -1)
            perror("send");

        if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        printf("Received: %s",buf);

        close(sockfd);
    }
}
