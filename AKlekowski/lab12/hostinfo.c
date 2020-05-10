/* Program hostinfo.c powinien wyświetlać informacje na temat:
    nazwy hosta, jego aliasów i przypisanych adresów IP
    dostępnych sieci
    dostępnych usług wraz z portami, na których działają
    dostępnych protokołów
    dostępnych interfejsów sieciowych. */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>


void display_hostent(struct hostent *h) {
    int i;

    printf("name: %s\n", h->h_name);
    printf("\ttype: %d\n", h->h_addrtype);
    printf("\tlen: %u\n", h->h_length);
    printf("\taliases: ");
    for (i=0; h->h_aliases[i] != NULL; i++)
        printf("%s\n", h->h_aliases[i]);
    printf("\n");
    if (h->h_addrtype == AF_INET) {
        for (i=0; h->h_addr_list[i] != NULL; i++)
            printf("\t%s\n", inet_ntoa(*((struct in_addr *)h->h_addr_list[i])));
    }
}

void hostdb() {
    struct hostent *h;

    sethostent(1);
    while ((h = gethostent())!= NULL)
        display_hostent(h);
    endhostent();
}

void display_netent(struct netent *n) {
    int i;

    printf("name: %s\n", n->n_name);
    printf("\taliases: ");
    for(i=0; n->n_aliases[i] != NULL; i++)
        printf("%s ", n->n_aliases[i]);
    printf("\n\taddres type: %d\n", n->n_addrtype);
    printf("\tnet num: %u\n", n->n_net);
}

void ndb() {
    struct netent *n;

    setnetent(1);
    while ((n = getnetent()) != NULL)
        display_netent(n);
    endnetent();
}

void display_servent(struct servent *s) {
    int i;

    printf("\nname: %s\n", s->s_name);
    printf("\taliases: ");
    for(i=0; s->s_aliases[i] != NULL; i++)
        printf("%s ", s->s_aliases[i]);
    printf("\n\tport: %i", s->s_port);
    printf("\n\tprotocol: %s\n", s->s_proto);
}

void sdb() {
    struct servent *s;

    setservent(1);
    while ((s = getservent()) != NULL)
        display_servent(s);
    endservent();
}

void pdb() {
    int i;
    struct protoent *pp;

    while(1) {
        if ( (pp = getprotoent()) == NULL ) {
            break;
        }

        printf("%s:\n"
               "\tProtocol: %d\n"
               "\tAliases: ",
               pp->p_name,
               pp->p_proto);

        for ( i=0; pp->p_aliases[i] != NULL; ++i ) {
            printf("%s ",pp->p_aliases[i]);
        }
        printf("\n");
    }
}

void ifdb(){
    struct if_nameindex *ni;
    int i;

    ni = if_nameindex();
    printf("network interfaces:\n");
    for (i=0; ni[i].if_index !=0 || ni[i].if_name != NULL; i++)
        printf("index: %d; name: %s\n", ni[i].if_index, ni[i].if_name);
    if_freenameindex(ni);
}

int main(int argc, char *argv[]) {
    if (argc <= 1)
    {
        fprintf(stderr,"usage: %s -h|-n|-s|-p|-i\n", argv[0]);
        return 1;
    }

    int i;
    for (i=0; i<argc; i++) {
        if (strcmp(argv[i], "-h") == 0)
            hostdb();
        if (strcmp(argv[i], "-n") == 0)
            ndb();
        if (strcmp(argv[i], "-s") == 0)
            sdb();
        if (strcmp(argv[i], "-p") == 0)
            pdb();
        if (strcmp(argv[i], "-i") == 0)
            ifdb();
    }
    return 0;
}
