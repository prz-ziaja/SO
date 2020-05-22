//
// Created by Maryna Lukachyk on 2020-05-21.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>

#define PORT 80
#define MAXDATASIZE 8192

int main(void)
{
    char url[100], s[500], *p;
    int sockfd;
    ssize_t numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in dest_addr;

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

        if ((he=gethostbyname(url)) == NULL) {  // get the host info
            perror("gethostbyname");
            exit(1);
        }

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        dest_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof dest_addr) == -1) {
            perror("connect");
            exit(1);
        }

        snprintf(s, sizeof(s), "GET / HTTP/1.0\r\n\r\n");

        if (send(sockfd, s, sizeof(s), 0) == -1)
            perror("send");

        if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';

        printf("Received: %s\n",buf);

        close(sockfd);
    }
}