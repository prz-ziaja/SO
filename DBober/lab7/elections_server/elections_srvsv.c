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

int winner(int committies[]){
	int max=0, index;
	for(int i=0; i<MAX_COMMITTIES; i++){
		if(committies[i] > max){
			max = committies[i];
			index = i;
		}
	}
	return index+1;
}

int main(void)
{
  struct message msg;
  char svsrvname[SRV_KEYFILE_PATH_SIZE];
  ssize_t brcv,
    bsnd;
  int kfor,    /* Key File Opening Result */
    svsrvqid;
  key_t qkey;
	int votes=0;
	int committies[MAX_COMMITTIES] = {0};
	long clientIDs[MAX_CLIENTS];

  printf("Server started...\n");
  setbuf(stdout, NULL);
  msg.clientid = -1;

  /* Creating server key file */
  printf("Creating server key file:\n");
  make_srv_sv_queue_name(svsrvname, SRV_KEYFILE_PATH_SIZE);
  printf("\tPath: %s\n", svsrvname);
  printf("\tOpening server key file...");
  kfor = open(svsrvname, O_WRONLY | O_CREAT, PERM_FILE);
  if(kfor == -1)
    {
      printf("FAIL!\nError: %s\n", svsrvname, strerror(errno));
      return 0;
    }
  close(kfor);
  printf("OK\n");

  /* Getting queue key */
  printf("Getting queue key...");
  qkey = ftok(svsrvname, 1);
  if(qkey == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }
  printf("OK\n");
  
  /* Getting server queue identifier */
  printf("Getting server queue identifier...");
  svsrvqid = msgget(qkey, PERM_FILE);
  if(svsrvqid == -1)
    {
      printf("the queue already exists, removing it...");
      msgctl(svsrvqid, IPC_RMID, NULL);
      svsrvqid = msgget(qkey, IPC_CREAT | PERM_FILE);
    }
  if(svsrvqid == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }
  printf("OK\n");

	int clientNum=0;
  while(1)
    {
      /* Reading from queue */
      for(int i=0; i<MAX_COMMITTIES;i++){
		    brcv = msgrcv(svsrvqid, &msg, MESSAGE_BUF_SIZE, 0, 0);
		    if(brcv == -1)
					break;

				(i!=0) ?:printf("Data from client [%ld] \n", msg.clientid);
				votes += atoi(msg.data);
				committies[msg.msgtype - 1] += atoi(msg.data);
		    printf("Committiee: %ld \nVotes: %s \n", msg.msgtype, msg.data);
		}
			clientIDs[clientNum] = msg.clientid;
			clientNum++;
      /* Responce getting and sending */
      snprintf(msg.data, MESSAGE_BUF_SIZE, "%d", votes);

      printf("Writting responce to clients...\n");snprintf(msg.data, MESSAGE_BUF_SIZE, "%d", votes);
			for( int i=0; i<clientNum; i++){
		    bsnd = msgsnd(clientIDs[i], &msg, MESSAGE_BUF_SIZE, 0);
		    if(bsnd == -1)
					{
						printf("FAIL!\nError: %s\n", strerror(errno));
						break;
					}
			}
			if(clientNum == MAX_COMMITTIES){
				snprintf(msg.data, MESSAGE_BUF_SIZE, "%s","end");
				for( int i=0; i<clientNum; i++){
				  bsnd = msgsnd(clientIDs[i], &msg, MESSAGE_BUF_SIZE, 0);
				  if(bsnd == -1){
							printf("FAIL!\nError: %s\n", strerror(errno));
							break;
					}
				}
				printf("Results: \n");
				for(int i=0; i<MAX_COMMITTIES; i++)
					printf("	Committee %d : %d \n", i+1, committies[i]);
				printf("Attendance: %d \nWinning committee: %d \n", votes, winner(committies));
				break;
			}
    }

  /* Cleaning up */
  unlink(svsrvname);
  msgctl(svsrvqid, IPC_RMID, NULL);
  for( int i=0; i<clientNum; i++)
    msgctl(clientIDs[i], IPC_RMID, NULL);
  
  return 0;
}
