#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "sv.h"
/* ------------------------------------------------------------- */

int main(void)
{
  struct message msg;
  char svsrvname[SRV_KEYFILE_PATH_SIZE];
  ssize_t brcv,
    bsnd;
  int svsrvqid,
    svcntqid;
  key_t srvqkey;
	
	srand(time(NULL));
	int votes;
	long committee = rand() % MAX_COMMITTIES + 1;

  printf("Client started...\n");
  setbuf(stdout, NULL);
	
  /* Creating server key file name */
  printf("Creating server key file name:\n");
  make_srv_sv_queue_name(svsrvname, SRV_KEYFILE_PATH_SIZE);
  printf("\tPath: %s\n", svsrvname);

  /* Getting server queue key */
  printf("Getting server queue key...");
  srvqkey = ftok(svsrvname, 1);
  if(srvqkey == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }
  printf("OK\n");
  
  /* Getting server queue identifier */
  printf("Getting server queue identifier...");
  svsrvqid = msgget(srvqkey, 0);
  if(svsrvqid == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }
  printf("OK\n");

  /* Getting client queue identifier */
  printf("Getting client queue identifier...");
  svcntqid = msgget(IPC_PRIVATE, PERM_FILE);
  if(svcntqid == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }
  printf("OK\n");
	
		for(int i=0; i<MAX_COMMITTIES;i++){
			votes = rand() % (MAX_VOTES/MAX_CLIENTS)/MAX_COMMITTIES;
      printf("Your message \n	Votes: %d \n	Committee: %d \n", votes, i+1);
      snprintf(msg.data, MESSAGE_BUF_SIZE, "%d", votes);
      msg.clientid = svcntqid;
      msg.msgtype = i+1;

      printf("Sending to server...");
      bsnd = msgsnd(svsrvqid, &msg, MESSAGE_BUF_SIZE, 0);
      if(bsnd == -1)
				{
					printf("FAIL!\nError: %s\n", strerror(errno));
					msgctl(svsrvqid, IPC_RMID, NULL);
  				msgctl(svcntqid, IPC_RMID, NULL);
					exit(EXIT_FAILURE);
				}
      printf("OK\n");
		}

  while(1)
  {
      /* Reading from queue */
      printf("Waiting for data...\n");
      brcv = msgrcv(svcntqid, &msg, MESSAGE_BUF_SIZE, 0, 0);
      if(brcv == -1)
	{
	  printf("FAIL!\nError: %s\n", strerror(errno));
	  break;
	}
      if( (strcmp(msg.data, "end") == 0))
				break;
			printf("Current attendance: %s\n", msg.data);
    }

  /* Cleaning up */
  msgctl(svsrvqid, IPC_RMID, NULL);
  msgctl(svcntqid, IPC_RMID, NULL);
  
  return 0;
}
