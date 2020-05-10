/*  Zmodyfikuj program tak, aby w zależności od typu błędu zwróconego przez funkcję getaddrinfo
wyświetlał odpowiedni komunikat (Jak funkcja służy do odczytania błędów związanych z funkcją getaddrinfo?)
Zmodyfikuj program tak, aby wyświetlał wszystkie adresy IP powiązane z danym adresem, a nie tylko pierwszy z nich.
Zmodyfikuj program tak, aby wyświetlał nazwę kanoniczną hosta.
Podpowiedź: Jaką flagę należy ustawić w strukture hints? */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>

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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        if (status == EAI_AGAIN)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_BADFLAGS)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_FAIL)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_FAMILY)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_MEMORY)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_NONAME)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_SERVICE)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_SOCKTYPE)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        if (status == EAI_SYSTEM)
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    void *addr;
    char *ipver;

    for(p=res; p != NULL; p=p->ai_next) {
        printf("%s\t", p->ai_canonname);

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
    exit(EXIT_SUCCESS);
}
