//
// Created by Maryna Lukachyk on 2020-06-03.
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

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

int main(int argc, char *argv[]){
    hostdb();
    return 0;
}