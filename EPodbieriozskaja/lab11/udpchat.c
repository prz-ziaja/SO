#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT 8295	// the port users will be connecting to

#define MAXBUFLEN 100

int main(int argc, char *argv[]) {
    int sockfd, pid;
    struct sockaddr_in my_addr;	// my address information
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    socklen_t addr_len;
    int numbytes;
    char buf[MAXBUFLEN];
    long port1, port2;


    if (argc != 5) {
        fprintf(stderr,"usage: udpchat Address1 Port1 Address2 Port2\n");
        exit(1);
    }
    sscanf(argv[2], "%ld", &port1);
    sscanf(argv[4], "%ld", &port2);

    if ((he=gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, getprotobyname("UDP")->p_proto)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port1);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(port2);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
        perror("bind");
        exit(1);
    }

    if ((pid = fork()) == -1) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        while(1){
            if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                                     (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("recvfrom");
                exit(1);
            }
            buf[numbytes] = '\0';
            printf("%s\n", buf);
        }
    } else {
        while(1){
            scanf("%s\n", buf);
            if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
                                   (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
                perror("sendto");
                exit(1);
            }
        }
    }

    close(sockfd);

    return 0;
}