#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>

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
  struct message msg;
  pid_t okregi[MAX_CLIENTS];
  int votes[MAX_COMITTIES];
  int lOkregow=0, licz, frekwencja=0, nrKomitetu;
  
  printf("Server started...\n");
  setbuf(stdout, NULL);
  msgsize = sizeof(msg);

  /* Creating server psx queue */
  make_srv_psx_name(srvpsxqname, srvpsxqbasename, PSX_QUEUE_NAME);

  /* Opening server psx queue */
  printf("Opening server queue \'%s\' for reading...", srvpsxqname);
  mq_unlink(srvpsxqname);
  qdsrv = mq_open(srvpsxqname, O_RDONLY | O_CREAT, PERM_FILE, NULL);
  if(qdsrv == -1)
    {
      printf("FAIL!\nError: %s.\n", strerror(errno));
      return 0;
    }
  printf("OK\n");

  while(1)
    {

	for(int j=0; j< MAX_COMITTIES; j++)
	{
	      /* Reading from queue */
	      printf("Waiting for data...\n");
	      bread = mq_receive(qdsrv, (char*)&msg, MESSAGE_BUF_SIZE, &nrKomitetu);
	      if(bread == -1)
		{
		  printf("FAIL!\nError: %s\n", strerror(errno));
		  break;
		}
	      printf("Message from [%ld]: %s\n", (long)msg.pid, msg.data);
		
		frekwencja+=atoi(msg.data);
		votes[nrKomitetu]+=atoi(msg.data);
	}

	okregi[lOkregow++] = msg.pid;

	if(lOkregow == MAX_CLIENTS)
	{
		snprintf(msg.data, MESSAGE_BUF_SIZE, "\nKoncowa frekwencja: %d\n", frekwencja);
		printf("%s",msg.data);


		int max = 0;

		for(int j=MAX_COMITTIES-1;j>=0;j--)
		{
			printf("Komitet nr: %d glosy: %d\n", j, votes[j]);

			if(votes[j]>votes[max])
				max=j;
		}


		printf("Zwyciezca: komitet nr %d\n\n",max);

	for(licz = lOkregow; licz > 0;)
	   {
	      /* Creating client queue name */
	      make_cnt_psx_name(cntpsxqname, okregi[--licz], PSX_QUEUE_NAME);

	      /* Opening client fifo for writing  */
	      printf("Opening client queue \'%s\' for writing...", cntpsxqname);
	      qdcnt = mq_open(cntpsxqname, O_WRONLY);
	      if(qdcnt == -1)
		{
		  printf("FAIL!\nError: %s\n", strerror(errno));
		  break;
		}

	      /* Sending responce */
	      printf("OK\nWritting responce to client %ld...", (long)msg.pid);
	      bwrite = mq_send(qdcnt, (const char*)&msg, MESSAGE_BUF_SIZE, 0);
	      if(bwrite == -1)
		{
		  printf("FAIL!\nError: %s\n", strerror(errno));
		  break;
		}
	      printf("OK\n");
	      mq_close(qdcnt);
	   }
		break;
	}

	for(licz = lOkregow; licz > 0; )
	{
		      /* Getting responce */
		      printf("\nAktualna frekwencja: %d\n\n",frekwencja);
		      snprintf(msg.data, MESSAGE_BUF_SIZE, "Aktualna frekwencja: %d\n", frekwencja);

		      /* Creating client queue name */
		      make_cnt_psx_name(cntpsxqname, okregi[--licz], PSX_QUEUE_NAME);

		      /* Opening client fifo for writing  */
		      printf("Opening client queue \'%s\' for writing...", cntpsxqname);
		      qdcnt = mq_open(cntpsxqname, O_WRONLY);
		      if(qdcnt == -1)
			{
			  printf("FAIL!\nError: %s\n", strerror(errno));
			  break;
			}

		      /* Sending responce */
		      printf("OK\nWritting responce to client %ld...", (long)msg.pid);
		      bwrite = mq_send(qdcnt, (const char*)&msg, MESSAGE_BUF_SIZE, 0);
		      if(bwrite == -1)
			{
			  printf("FAIL!\nError: %s\n", strerror(errno));
			  break;
			}
		      printf("OK\n");
		      mq_close(qdcnt);	

	}



	
	
    }
  
  
  /* Cleaning up */
  mq_close(qdsrv);
  mq_unlink(srvpsxqname);
  
  return 0;
}
