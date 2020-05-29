#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define MAXBUFLEN 1000
#define MAXNAMELEN 20
#define MAXCLIENTS 10
#define MAXMEETINGS 4

#define SERVER_PORT 5555

struct meeting
{
	char text[MAXNAMELEN];
	time_t time;
	int notified;
};

struct client
{
	struct in_addr address;
	unsigned short port;
	char name[MAXNAMELEN];
	struct meeting meetings[MAXMEETINGS];
	int nmeetings;
};

struct client clients[MAXCLIENTS];
int nclients, sockfd;

int send_msg(struct in_addr address, unsigned short port, char *buf, int numbytes)
{
	struct sockaddr_in their_addr_w;

	their_addr_w.sin_family = AF_INET;
	their_addr_w.sin_addr = address;
	their_addr_w.sin_port = port;
	memset(their_addr_w.sin_zero, '\0', sizeof their_addr_w.sin_zero);

	printf("sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr_w.sin_addr));

	return sendto(sockfd, buf, numbytes, 0, (struct sockaddr *)&their_addr_w, sizeof their_addr_w);
}

void* sending(void* arg)/* sending data to clients */
{
	int i, j, prev_day=0, curr_day=0;
	char buf[MAXBUFLEN];
	time_t curr_time;
	
	while(1)
	{
		curr_time = time(NULL);
		prev_day = curr_day;
		curr_day = curr_time / 86400;

		if(prev_day < curr_day) /* informuje ze nadszedl kolejny dzien */
		{
			strcpy(buf,"Nadszedl kolejny dzien!\n");

			for(i = 0; i < nclients; i++)
			{
				if (send_msg(clients[i].address,clients[i].port,buf,strlen(buf)) == -1)
				{
					perror("sendto");
					exit(1);
				}	
			}
	
		}

		for(i = 0; i < nclients; i++) /* informuje o spotkaniach */
		{
			for(j = 0; j < clients[i].nmeetings; j++)
			{
				if(!clients[i].meetings[j].notified && clients[i].meetings[j].time > curr_time && clients[i].meetings[j].time < curr_time + 60)
				{
					sprintf(buf,"%s zbliza sie termin spotkania!\n%ld %s\n", clients[i].name, clients[i].meetings[j].time, clients[i].meetings[j].text);
					
					if (send_msg(clients[i].address,clients[i].port,buf,strlen(buf)) == -1)
					{
						perror("sendto");
						exit(1);
					}

					clients[i].meetings[j].notified = 1;				
				}
			}
		}

		sleep(5);
	}
}
void* receiving(void* arg)/* receiving data from clients */
{
	int numbytes;
	char buf[MAXBUFLEN];
	struct sockaddr_in their_addr_r;
	socklen_t addr_len = sizeof their_addr_r;

	while(1)
	{
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr_r, &addr_len)) == -1)
		{
			perror("recvfrom");
			exit(1);
		}
		
		buf[numbytes] = '\0';

		printf("got %d bytes packet from %s on port %d: \"%s\"\n",numbytes,inet_ntoa(their_addr_r.sin_addr),ntohs(their_addr_r.sin_port),buf);

		if(strstr(buf,"REGISTER")) /* send initial message */
		{
			clients[nclients].address = their_addr_r.sin_addr;
			clients[nclients].port = their_addr_r.sin_port;
			strcpy(clients[nclients].name,buf+strlen("REGISTER"));
			
			sprintf(buf,"Witaj %s!\nDostepne komendy:\na) wyswietl liste spotkan uzytkownika x - SPOTKANIA X\nb) dodaj spotkanie dla uzytkownika x - DODAJ X CZAS OPIS\nc) wyswietl uzytkownikow - LISTA\n",clients[nclients].name);

			if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
			{
				perror("sendto");
				exit(1);
			}

			nclients++;
			continue;
		}
		
		if(strstr(buf,"LISTA")) /* send list of users */
		{
			int i;

			strcpy(buf,"Lista uzytkownikow:\n");

			for(i=0;i<nclients;i++)
			{
				strcat(buf,clients[i].name);
				strcat(buf,"\n");
			}
				
			if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
			{
				perror("sendto");
				exit(1);
			}

			continue;
		}

		if(strstr(buf,"DODAJ")) /* add new meeting */
		{
			int i = 0, found = -1;
			char words[4][MAXNAMELEN];
			char buf2[MAXBUFLEN];			
			char *pch;

			strcpy(buf2,buf);

			pch = strtok(buf," ");
			for(i=0;i<3;i++)
			{
				strcpy(words[i],pch); 
				pch = strtok(NULL, " ");
			}

			strcpy(words[i],buf2+(pch-buf));

			for(i=0;i<nclients;i++)
			{
				if(strcmp(clients[i].name, words[1]) == 0)
				{
					found = i;
					break;
				}
			}

			if(found == -1)
			{
				strcpy(buf,"Nie ma takiego uzytkownika!");

				if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
				{
					perror("sendto");
					exit(1);
				}	
				continue;
			}
			
			clients[found].meetings[clients[found].nmeetings].time = atoi(words[2]);
			strcpy(clients[found].meetings[clients[found].nmeetings].text, words[3]);
			clients[found].meetings[clients[found].nmeetings].notified = 0;
			
			clients[found].nmeetings++;

			sprintf(buf,"Dodano spotkanie do kalendarza uzytkownika %s!\n",words[1]);

			if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
			{
				perror("sendto");
				exit(1);
			}	

			continue;
		}

		if(strstr(buf,"SPOTKANIA")) /* view user meetings */
		{
			int i = 0, found = -1;
			char words[2][MAXNAMELEN];
			char buf2[MAXBUFLEN];
			
			char *pch;

			pch = strtok(buf," ");
			while(pch != NULL)
			{
				strcpy(words[i++],pch);
				pch = strtok(NULL, " ");
			}

			for(i=0;i<nclients;i++)
			{
				if(strcmp(clients[i].name, words[1]) == 0)
				{
					found = i;
					break;
				}
			}

			if(found == -1)
			{
				strcpy(buf,"Nie ma takiego uzytkownika!");

				if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
				{
					perror("sendto");
					exit(1);
				}	
				continue;
			}
			
			sprintf(buf,"Spotkania uzytkownika %s:\n",words[1]);

			for(i=0;i<clients[found].nmeetings;i++)
			{
				sprintf(buf2,"%d) %ld %s\n",i+1,clients[found].meetings[i].time, clients[found].meetings[i].text);
				strcat(buf,buf2);
			}

			if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
			{
				perror("sendto");
				exit(1);
			}	

			continue;
		}

		strcpy(buf,"Nierozpoznana komenda!");
		if(send_msg(their_addr_r.sin_addr,their_addr_r.sin_port,buf,strlen(buf)) == -1)
		{
			perror("sendto");
			exit(1);
		}	
	}
}

int main(int argc, char *argv[])
{
	int i, yes = 1;
	struct sockaddr_in my_addr;

	if (argc != 1) {
		fprintf(stderr,"usage: kalendarz_serwer\n");
		exit(1);
	}

	for(i=0;i<MAXCLIENTS;i++)
		clients[i].nmeetings=0;

	my_addr.sin_family = AF_INET;		 
	my_addr.sin_port = htons(SERVER_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		perror("bind");
		exit(1);
	}

	nclients = 0;

	pthread_t threads[2];
	pthread_create(&threads[0],NULL,sending,NULL);
	pthread_create(&threads[1],NULL,receiving,NULL);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	close(sockfd);

	return 0;
}
