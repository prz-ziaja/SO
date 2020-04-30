#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
/* ------------------------------------------------------------- */

#include "pkw.h"
/* ------------------------------------------------------------- */

int main(void)
{
  char* srvpsxqbasename = "psxsrvqueue";
  char srvpsxqname[PSX_QUEUE_NAME];
  char cntpsxqname[PSX_QUEUE_NAME];
  mqd_t qdsrv,
    qdcnt;
  ssize_t msgsize,
    bread,
    bwrite;
  pid_t pid;
  struct message msg;
  int i;

  srand(time(NULL));
  
  printf("Server started...\n");
  setbuf(stdout, NULL);
  msgsize = sizeof(msg);
  pid = getpid();
  msg.pid = pid;

  /* Creating server psx queue */
  make_srv_psx_name(srvpsxqname, srvpsxqbasename, PSX_QUEUE_NAME);

  /* Opening server queue for writing  */
  printf("Opening server queue \'%s\' for writing...", srvpsxqname);
  qdsrv = mq_open(srvpsxqname, O_WRONLY);
  if(qdsrv == -1)
    {
      printf("FAIL!\nError: %s.\n", strerror(errno));
      return 0;
    }
  printf("OK\n");

  /* Creating client psx queue name */
  make_cnt_psx_name(cntpsxqname, pid, PSX_QUEUE_NAME);
  
  /* Opening client queue for reading  */
  printf("Opening client queue \'%s\' for reading...", cntpsxqname);
  mq_unlink(cntpsxqname);
  qdcnt = mq_open(cntpsxqname, O_RDONLY | O_CREAT, PERM_FILE, NULL);
  if(qdcnt == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }      
  printf("OK\n");


      /* Sending message */
      printf("Writting message...\n\n");

	int votes[MAX_COMITTIES];
	int votesLeft = MAX_VOTES/MAX_CLIENTS;

	for(int j=MAX_COMITTIES-1; j >= 0; j--)
	{
		votes[j]=rand() % (votesLeft+1);
		votesLeft-=votes[j];	

		printf("j=%d votes=%d\n",j,votes[j]);

		snprintf(msg.data, MESSAGE_BUF_SIZE, "%d", votes[j]);
		bwrite = mq_send(qdsrv, (const char*)&msg, MESSAGE_BUF_SIZE, j);
	        if(bwrite == -1)
		{
		  printf("FAIL!\nError: %s\n", strerror(errno));
		  return 1;
		}

	}


      printf("\nOK\n");

  while(1)
    {

      /* Reading from queue */
      printf("Waiting for data...\n");
      bread = mq_receive(qdcnt, (char*)&msg, MESSAGE_BUF_SIZE, NULL);
      if(bread == -1)
	{
	  printf("FAIL!\nError: %s\n", strerror(errno));
	  break;
	}

      printf("Message from server: %s\n", msg.data);

	if(strstr(msg.data,"Koncowa frekwencja"))
		break;
    }
  
  /* Cleaning up */
  mq_close(qdsrv);
  mq_close(qdcnt);
  
  return 0;
}
