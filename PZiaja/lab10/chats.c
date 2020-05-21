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
     
#define MYPORT 3187
     
#define BACKLOG 10	 // how many pending connections queue will hold

#define BUF_LEN 100    
void sigchld_handler(int s)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

    
int main(void)
{
  int sockfd, fd_client, fd_hwm=0,fd;  // listen on sock_fd, new connection on new_fd
  struct sockaddr_in my_addr;	// my address information
  struct sockaddr_in their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  int pid,pid2;
  int bread;
  char buf[BUF_LEN];
  char buf2[BUF_LEN];
  fd_set set, read_set;
  
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
     
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }
     
  my_addr.sin_family = AF_INET;		 
  my_addr.sin_port = htons(MYPORT);	 
  my_addr.sin_addr.s_addr = INADDR_ANY; 
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);
     
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
    perror("bind");
    exit(1);
  }
     
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  if(sockfd >fd_hwm)
    fd_hwm = sockfd;
  FD_ZERO(&set);
  FD_SET(sockfd, &set);

  sa.sa_handler = sigchld_handler; 
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
  
  while(1){
    read_set = set;
    if( select(fd_hwm+1, &read_set, NULL, NULL, NULL) == -1)
      perror("select");
  
    for(fd =0; fd<=fd_hwm;fd++){
   
      if(FD_ISSET(fd, &read_set)){
	
	if(fd == sockfd){
	  sin_size = sizeof their_addr;
	  if ((fd_client = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
	    perror("accept");
	  }
	  FD_SET(fd_client, &set);

	  if(fd_client >fd_hwm)
	    fd_hwm = fd_client;
	}
	else{

	  if((pid = fork()) == -1)
	    perror("fork");
	  if(pid>0){
	   
	    if((pid2 = fork()) == -1)
	      perror("fork2");
	    if(pid2 == 0){
	      while(1){
		if( (bread = read(fd,buf, BUF_LEN-1)) == -1){
		  perror("read");
		  exit(1);
		}
		if(bread > 0)
		  printf("Server got %s\n", buf);
	      }
	    }
	    else{
	      while(1){
		scanf("%s", buf2);
		if (send(fd, buf2, BUF_LEN-1, 0) == -1){
		  perror("send");
		  exit(1);
		}
	      }
	    }
	  } 
	}
      }
    

    }
    
   
     
    

   
   
    /* sleep(5); // just for observing easily that the server cannot serve a few clients concurrently */
    /* printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr)); */
        
    /* close(new_fd); */
  

  }
  return 0;
}
