    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <netdb.h>
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
        struct netent* networks;
        setnetent(1);
        while((networks = getnetent()) != NULL)
        {
            printf("Siec: %s\n", networks->n_name);
        }
        endnetent();

        struct servent* service;
        setservent(1);
        while((service = getservent()) != NULL)
        {
            printf("Serwis: %s\n", service->s_name);
        }
        endservent();

        setprotoent(1);
        struct protoent* protocol;
        while(protocol = getprotoent())
        {
            printf("Protokol: %s\n", protocol->p_name);
        }
        endprotoent();

        struct if_nameindex* interfaces;
        interfaces->if_name;
        interfaces = if_nameindex();
        int index = 0;
        while(interfaces[index].if_index != 0)
        {
            printf("Interfejs: %s\n", interfaces[index].if_name);
            index++;
        }
        if_freenameindex(interfaces);

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

