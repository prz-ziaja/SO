#include <stdio.h>
#include <errno.h>
#include <netdb.h>      /* 4 gethostbyname, hostent structure */
#include <unistd.h>     /* 4 exit */
#include <netinet/in.h> /* 4 ntohn */


int main(int argc, char *argv[])
{
     int i,j;
     struct hostent *he;

     if (argc != 2) 
     {
          fprintf(stderr,"usage: %s hostname\n", argv[0]);
          return 1;
     }

     if ((he = gethostbyname(argv[1])) == NULL) 
     {  
        if (h_errno == NO_RECOVERY)
            printf("Server can't return address\n.");
        else if (h_errno == TRY_AGAIN)
            printf("Problem with server.\n");
        else if(h_errno == HOST_NOT_FOUND)
            printf("The host does not exist!\n");

          return 1;
     }

     /* host info: */
     printf("\nHost name: %s", he->h_name);
     printf("\nAliases:");
     for(i=0;he->h_aliases[i] != NULL;++i)
          printf("\n%d. %s", i+1, he->h_aliases[i]);
     if(he->h_addrtype == AF_INET)
          printf("\nAddres type: IPv4");
     if(he->h_addrtype == AF_INET6)
          printf("\nAddres type: IPv6");
     printf("\nAddress length: %d bytes", he->h_length);
     printf("\nAddresses:");
     for(i=0;he->h_addr_list[i]!=0;i++){
	     for(j=0;j<he->h_length;++j)
	     {
		  printf("%d", (uint8_t)he->h_addr_list[i][j]);
		  if(j < (he->h_length-1))
		       printf(".");
	     }
     printf("\n");}

     return 0;
}
