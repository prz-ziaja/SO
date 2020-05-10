#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

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
        fprintf(stderr,"usage: udpChat myAddress myPort friendAddress friendPort\n");
        exit(EXIT_FAILURE);
    }

    sscanf(argv[2], "%ld", &port1);
    sscanf(argv[4], "%ld", &port2);
    inet_pton(AF_INET, argv[1], &(my_addr.sin_addr.s_addr));
    inet_pton(AF_INET, argv[3], &(their_addr.sin_addr.s_addr));

    if ((he=gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, getprotobyname("UDP")->p_proto)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    int p_pid = getpid();

    if ((pid = fork()) == -1) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        while(1){
            if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                                     (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }
            buf[numbytes] = '\0';
            printf(">%s", buf);

            if (strcmp(buf, "Bye\n") == 0 || strcmp(buf, "bye\n") == 0) {
                kill(pid, SIGKILL);
                close(sockfd);
                exit(EXIT_SUCCESS);
            }
        }
    } else {
        while(1){
            if (fgets(buf, MAXBUFLEN, stdin) == NULL) {
                perror("fgets");
                continue;
            }

            if ((sendto(sockfd, buf, strlen(buf), 0,
                                   (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
                perror("sendto");
                exit(EXIT_FAILURE);
            }

            if (strcmp(buf, "Bye\n") == 0 || strcmp(buf, "bye\n") == 0) {
                kill(p_pid, SIGKILL);
                close(sockfd);
                exit(EXIT_SUCCESS);
            }
        }
    }
}

