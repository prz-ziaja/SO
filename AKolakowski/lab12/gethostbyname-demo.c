#define _DEFAULT_SOURCE

#include <stdio.h>
#include <errno.h>
#include <netdb.h>      /* 4 gethostbyname, hostent structure */
#include <unistd.h>     /* 4 exit */
#include <netinet/in.h> /* 4 ntohn */
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
     int i;
     struct hostent *he;

     if (argc != 2) 
     {
          fprintf(stderr,"usage: %s hostname\n", argv[0]);
          return 1;
     }

     if ((he = gethostbyname(argv[1])) == NULL) 
     {  
          herror("gethostbyname");
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
     printf("\nAddresses:\n");
     for (i=0; he->h_addr_list[i] != NULL; i++)
          printf("\t%s\n", inet_ntoa(*((struct in_addr *)he->h_addr_list[i])));
     printf("\n");

     return 0;
}
