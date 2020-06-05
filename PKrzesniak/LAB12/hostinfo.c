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

void display_netent(struct netent *h){
	int i;
	
	printf("name: %s; type: %d; number: %u\n", h->n_name, h->n_addrtype, h->n_net);
	for (i=0; h->n_aliases[i] != NULL; i++)
		printf("\t%s\n", h->n_aliases[i]);
}

void display_servent(struct servent *h){
	int i;
	
	printf("name: %s; port: %d; protocol: %s\n", h->s_name, h->s_port, h->s_proto);
	for (i=0; h->s_aliases[i] != NULL; i++)
		printf("\t%s\n", h->s_aliases[i]);
}

void display_protoent(struct protoent *h){
	int i;
	
	printf("name: %s; number: %d\n", h->p_name, h->p_proto);
	for (i=0; h->p_aliases[i] != NULL; i++)
		printf("\t%s\n", h->p_aliases[i]);
}


int main(int argc, char *argv[]){
	printf("Network host entries:\n");
	struct hostent *host;
	struct netent *net;
	struct servent *serv;
	struct protoent *protocol;
	struct if_nameindex *ni;
	int i;
	
	sethostent(1);
	while ((host = gethostent())!= NULL)
		display_hostent(host);
	endhostent();

	printf("\nNetwork entries:\n");
		
	setnetent(1);
	while ((net = getnetent())!= NULL)
		display_netent(net);
	endnetent();

	printf("\nService entries:\n");
		
	setservent(1);
	while ((serv = getservent())!= NULL)
		display_servent(serv);
	endservent();

	printf("\nProtocol entries:\n");
	
	
	setprotoent(1);
	while ((protocol = getprotoent())!= NULL)
		display_protoent(protocol);
	endprotoent();

	printf("\nNetwork interface entries:\n");
	
	ni = if_nameindex();
	for (i=0; ni[i].if_index !=0 || ni[i].if_name != NULL; i++)
		printf("index: %d; name: %s\n", ni[i].if_index, ni[i].if_name);
	if_freenameindex(ni);

	return 0;
}
