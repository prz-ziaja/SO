#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 58394 /* the port client will be connecting to */

#define MAXDATASIZE 100 /* max number of bytes we can get at once */

int main(int argc, char *argv[]) {
    int sockfd, numbytes, pid;
    char buf[MAXDATASIZE];
    struct hostent * he;
    struct sockaddr_in their_addr; /* connector's address information */

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL) { /* get the host info */
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET; /* host byte order */
    their_addr.sin_port = htons(PORT); /* short, network byte order */
    their_addr.sin_addr = *((struct in_addr *) he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof their_addr) == -1) {
        perror("connect");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        while (1) {
            numbytes = recv(sockfd, buf, MAXDATASIZE, 0);
            if (numbytes > 0) {
                if (buf[0] == EOF) 
                    break;
                buf[numbytes] = '\0';
                printf("From server: %s \n", buf);
                fflush(stdout);
            } else if (numbytes < 0) {
                perror("READING");
                fflush(stdout);
            } else {
                break;
            }
        }
        close(sockfd);
        exit(EXIT_SUCCESS);
    }

    while (1) {
        scanf("%s", buf);
        if (!strcmp(buf, "break")) {
            buf[0] = EOF;
            if (send(sockfd, buf, strlen(buf), 0) == -1) {
                perror("send");
                exit(EXIT_FAILURE);
            }
            break;
        }
        if (send(sockfd, buf, strlen(buf), 0) == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);

    return 0;
}
