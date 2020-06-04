#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
void display_hostent(struct hostent *h){
    int i;
    printf("name: %s; type: %d; len: %d\n", h->h_name, h->h_addrtype, h->h_length);
    for (i=0; h->h_aliases[i] != NULL; i++)
        printf("\t%s\n", h->h_aliases[i]);
    if (h->h_addrtype == AF_INET) {
        for (i=0; h->h_addr_list[i] != NULL; i++)
            printf("\t%s\n", inet_ntoa(*((struct in_addr *)h->h_addr_list[i])));
    }
}
void hostdb(){
    struct hostent *h;
    sethostent(1);
    while ((h = gethostent())!= NULL)
        display_hostent(h);
    endhostent();
}
void net_diag(struct netent *h){
    int i;
    printf("name: %s\n\ttype: %d\n\tnumber: %u\n\taliases:\n", h->n_name, h->n_addrtype, h->n_net);
    for (i=0; h->n_aliases[i] != NULL; i++)
        printf("\t\t\s\n", h->n_aliases[i]);
}
void net() {
    struct netent *n;
    setnetent(1);
    while ((n = getnetent()) != NULL)
        net_diag(n);
    endnetent();
}
void d_serv(struct servent *s) {
    int i;
    printf("\nname: %s\n", s->s_name);
    printf("\taliases: ");
    for(i=0; s->s_aliases[i] != NULL; i++)
        printf("%s ", s->s_aliases[i]);
    printf("\n\tport: %i", s->s_port);
    printf("\n\tprotocol: %s\n", s->s_proto);
}
void services() {
    struct servent *s;
    setservent(1);
    while ((s = getservent()) != NULL)
        d_serv(s);
    endservent();
}
void d_prot(struct protoent *h){
    int i;
    printf("protocol: %s\naliases: %d\n", h->p_name, h->p_proto);
    for (i=0; h->p_aliases[i] != NULL; i++)
        printf("\t%s\n", h->p_aliases[i]);
}
void protocoles(){
    struct protoent *h;
    setprotoent(1);
    while ((h = getprotoent())!= NULL)
        d_prot(h);
    endprotoent();
}
void ifdb(){
    struct if_nameindex *ni;
    int i;
    ni = if_nameindex();
    for (i=0; ni[i].if_index !=0 || ni[i].if_name != NULL; i++)
        printf("index: %d; name: %s\n", ni[i].if_index, ni[i].if_name);
    if_freenameindex(ni);
}
int main(int argc, char *argv[]){
    printf("gethostent----------------------------------\n");
    hostdb();
    printf("getnetent-----------------------------------\n");
    net();
    printf("getservent----------------------------------\n");
    services();
    printf("getptoyoent---------------------------------\n");
    protocoles();
    printf("if_nameindex--------------------------------\n");
    ifdb();
    return 0;
}



