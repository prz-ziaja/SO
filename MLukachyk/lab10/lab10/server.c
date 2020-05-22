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
#include <fcntl.h>

#define MYPORT 308294	/* the port users will be connecting to */

#define BACKLOG 10	 /* how many pending connections queue will hold */
#define MAXDATASIZE 100

void setnonblocking(int sock) {
    int opts;

    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
}

void getNewConnection(int sockfd, int* connectlist) {
    int i, connection;
    connection = accept(sockfd, NULL, NULL);
    if (connection < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    setnonblocking(connection);
    for (i = 0; (i < BACKLOG) && (connection != -1); i++) {
        if (connectlist[i] == 0) {
            printf("%c %c", connection , i);
//                    );
            connectlist[i] = connection;
            connection = -1;
        }
    }
    if (connection != -1) {
        printf("\nNo room left for new client.\n");
        close(connection);
    }
}

void communicate(int fd) {
    int numbytes;
    char msg[MAXDATASIZE];
    numbytes = recv(fd, msg, MAXDATASIZE, 0);
    if (numbytes > 0) {
        if (msg[0] != EOF) {
            msg[numbytes] = '\0';
            printf("Received: %s \n", msg);
            fflush(stdout);
        }
        if ((numbytes = send(fd, "Confirmed", 10, 0)) == -1) {
            perror("send");
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
    } else if (numbytes < 0) {
        perror("READING");
    }
}

int getHighest(const int * connectlist, int prevHigh, int sockfd, fd_set* socks) {
    int i;

    FD_ZERO(socks);
    FD_SET(sockfd, socks);
    for (i = 0; i < BACKLOG; i++) {
        if (connectlist[i] != 0) {
            FD_SET(connectlist[i], socks);
            if (connectlist[i] > prevHigh)
                prevHigh = connectlist[i];
        }
    }
    return prevHigh;
}

int main(void) {
    int sockfd, i, yes = 1; /* listen on sock_fd */
    struct sockaddr_in addr; /* my address information */
    int connectlist[BACKLOG], readsocks, highsock;
    struct timeval timeout; /* Timeout for select */
    fd_set socks;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    setnonblocking(sockfd);
    addr.sin_family = AF_INET; /* host byte order */
    addr.sin_port = htons(MYPORT); /* short, network byte order */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* automatically fill with my IP */
    memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *) &addr, sizeof addr) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    highsock = sockfd;
    memset((char *) &connectlist, 0, sizeof (connectlist));
    while (1) {
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        highsock = getHighest(connectlist, highsock, sockfd, &socks);

        readsocks = select(highsock + 1, &socks, (fd_set *) 0,
                           (fd_set *) 0, &timeout);

        if (readsocks < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (readsocks > 0) {
            if (FD_ISSET(sockfd, &socks)) {
                getNewConnection(sockfd, connectlist);
            }
            for (i = 0; i < BACKLOG; i++) {
                if (FD_ISSET(connectlist[i], &socks)) {
                    communicate(connectlist[i]);
                }
            }
        }
    }
    return 0;
}
