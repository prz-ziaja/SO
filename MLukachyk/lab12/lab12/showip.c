//
// Created by Maryna Lukachyk on 2020-06-03.
//


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <net/if.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        if (status == EAI_AGAIN)
            printf("getaddrinfo: %s\n", gai_strerror(status));
        else if (status == EAI_BADFLAGS)
            printf("getaddrinfo: %s\n", gai_strerror(status));
        else if (status == EAI_FAIL)
            printf("getaddrinfo: %s\n", gai_strerror(status));
        else if (status == EAI_FAMILY)
            printf("getaddrinfo: %s\n", gai_strerror(status));
        else if (status == EAI_MEMORY)
            printf("getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    p = res;
    void *addr;
    char *ipver;
    while(p!= 0){
        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
        p=p->ai_next;
    }
    freeaddrinfo(res);



    return 0;
}

void ifdb(){
    struct if_nameindex *ni;
    int i;

    ni = if_nameindex();
    for (i=0; ni[i].if_index !=0 || ni[i].if_name != NULL; i++)
        printf("index: %d; name: %s\n", ni[i].if_index, ni[i].if_name);
    if_freenameindex(ni);
}