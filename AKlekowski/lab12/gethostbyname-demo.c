/*  Dopisać instrukcje, które szczegółowo sprawdzają typ błędu funkcji gethostbyname
i w zależności od tego wyświetlają odpowiedni komunikat.
Zmodyfikować tak program aby wyświetlał wszystkie adresy IP odnoszące się do podanego adresu. */

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i;
    struct hostent* host;
    struct sockaddr_in sock_addr;

    if (argc != 2)
    {
        fprintf(stderr,"usage: %s hostname\n", argv[0]);
        return 1;
    }

    if ((host = gethostbyname(argv[1])) == NULL)
    {
        if(h_errno == HOST_NOT_FOUND)
            fprintf(stderr,"The specified host is unknown.\n");
        if (h_errno == NO_DATA)
            perror("The requested name is valid but does not  have  an  IP address.");
        if (h_errno == NO_RECOVERY)
            perror("A nonrecoverable name server error occurred.");
        if (h_errno == TRY_AGAIN)
            perror("A  temporary  error  occurred on an authoritative name server.  Try again later.");
        exit(EXIT_FAILURE);
    }

    /* host info: */
    printf("\nHost name: %s", host->h_name);

    printf("\nAliases:");
    for(i=0;host->h_aliases[i] != NULL;++i)
        printf("\n%d. %s", i+1, host->h_aliases[i]);

    if(host->h_addrtype == AF_INET)
        printf("\nAddres type: IPv4");
    if(host->h_addrtype == AF_INET6)
        printf("\nAddres type: IPv6");

    printf("\nAddress length: %d bytes", host->h_length);

    printf("\nAddresses: ");
    for (i = 0; host->h_addr_list[i] != NULL; ++i) {
        sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);
        printf("%s\n", inet_ntoa(sock_addr.sin_addr));
    }

    return 0;
}
