#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
     
#define PORT 3080
     
#define MAXDATASIZE 100 // max number of bytes we can get at once 
     
int main(int argc, char *argv[])
{
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  char buf2[MAXDATASIZE];
  struct hostent *he;
  int pid;
  int bread;
  struct sockaddr_in their_addr; // connector's address information 
     
  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }
     
  if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info 
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
     
	
  if((pid = fork()) == -1)
    perror("fork");
  if(pid == 0){
    while(1){
      printf(">");
      scanf("%s", buf2);
      if (send(sockfd, buf2, MAXDATASIZE-1, 0) == -1)
	perror("send");
    }
  }
  else{
    while(1){
      if( (bread = read(sockfd,buf, MAXDATASIZE-1)) == -1)
	perror("read");
      if(bread>0)
	printf("Client got %s\n", buf);

    }

  }
	
     
  close(sockfd);
     
  return 0;
}