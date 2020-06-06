/*
** talker.c -- a datagram "client" demo
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
#include <netdb.h>
    
#define SERVERPORT 1234	// the port users will be connecting to
    
char hello[] = "Hello!";

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    
    if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }
    
    if ((he=gethostbyname(argv[1])) == NULL) 
    {  // get the host info
        perror("gethostbyname");
        exit(1);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }
    int broadcastEnable = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)));

    their_addr.sin_family = AF_INET;	 // host byte order
    their_addr.sin_port = htons(SERVERPORT); // short, network byte order
    //their_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
    memcpy(&(their_addr.sin_addr), *he->h_addr_list, sizeof(unsigned long));
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]) + 1, 
        0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr_in))) == -1) {
        perror("sendto");
        exit(1);
    }
    
    printf("sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));
    
    close(sockfd);
    
    return 0;
}
