#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
/* ------------------------------------------------------------- */

#include "fifo.h"

#define MAX 100
#define SHORT 10
#define VERYSHORT 2
/* ------------------------------------------------------------- */

char topics[MAX][80];
char token[80];
char *p_pos;
int res=0;
int head=0;
int tail=0;

char *response[]= {
     "How are you this beautiful day ?",
     "Did you have a happy childhood ?",
     "Did you hate your father ?",
     "Did you have a good friend ?",
     "Did you like your friend ?",
     "What do you think about your friendship ?",
     "I'm not sure I understand .",
     ""
};

char *trans[]={
     "no","Tell me more about the detail .",
     "yes","Tell me more about the detail .",
     "fuck","Don't be so rude again !",
     "shet","Don't be so rude again !",
     "you","Let't not talk about me .",
     "think","Why do you think that ?",
     "hate","So you hate something -- tell me more .",
     "what","Why do you ask ?",
     "want","Why do you want that ?",
     "need","We all need many things -- is this special ?",
     "why","Remember,therapy is good for you .",
     "know","How do you know that ?",
     "bye","Your bill will be mailed to you .",
     "murder","I don't like killing .",
     "kill","It is wrong to kill .",
     "jerk","Don't ever call me a jerk !",
     "can't","Don't be negative -- be positive .",
     "failure","Strive for success .",
     "never","Don't be negative -- be positive .",
     "unhappy","Why are you unhappy ?",
     "beautiful","I'm sure you like her,don't you ?",
     "like","Why do you like that?",
     "love","Remember,love everthing what you love .",
     ""
};

void assert_topic(char *);
void get_token(void);
int is_in(char, char *);
int lookup(char *);
int find_topic(char *);
int in_topics(char *);
void respond(char *);

char responce[MESSAGE_BUF_SIZE];

int main(void)
{
  struct message msg;
  char* fifosrvbasename = "srvfifoqueue";
  char fifosrvname[FIFO_NAME_BUF_SIZE];
  char fifocntname[FIFO_NAME_BUF_SIZE];
  int fdsrv,
    fdcnt,
    bread,
    bwrite,
    prev_flags,
    new_flags,
    counter = 0;
  
  printf("Server started...\n");
  setbuf(stdout, NULL);

  /* Creating server fifo queue */
  make_srv_fifo_queue_name(fifosrvname, fifosrvbasename, FIFO_NAME_BUF_SIZE);
  printf("Creating server fifo queue \'%s\'...", fifosrvname);
  if((mkfifo(fifosrvname, PERM_FILE) == -1) && (errno != EEXIST))
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }

  /* Opening fifo */
  printf("OK\nOpening server fifo queue \'%s\' for reading...", fifosrvname);
  fdsrv = open(fifosrvname, O_RDONLY | O_NONBLOCK);
  if(fdsrv == -1)
    {
      printf("FAIL!\nError: \'%s\'\n", strerror(errno));
      return 0;
    }
  printf("OK\n");

  prev_flags = fcntl(fdsrv,F_GETFL);
  new_flags = prev_flags & (~O_NONBLOCK);
  fcntl(fdsrv, F_SETFL, new_flags);

  /* simulate writer so read below doesn't return immediately */
  open(fifosrvname, O_WRONLY);

  while(1)
    {
      /* Reading from queue */
      printf("Waiting for data...");
      bread = read(fdsrv, &msg, sizeof(msg));
      if(bread == -1)
	{
	  printf("FAIL!\nError: %s\n", strerror(errno));
	  break;
	}
      printf("OK\n");

      printf("Message from client [%d]: %s\n", msg.pid, msg.data);
      printf("Auto-generated responce: ");

	if(counter == 0)
	{
		snprintf(msg.data, MESSAGE_BUF_SIZE, "%s\n",response[res++]);
		counter++;
	}
	else
	{
		p_pos = msg.data;
        	respond(msg.data);
		strncpy(msg.data, responce, MESSAGE_BUF_SIZE);
	}

     printf("%s\n",msg.data);

      /* Creating client fifo name  */
      make_cnt_fifo_queue_name(fifocntname, msg.pid, FIFO_NAME_BUF_SIZE); 

      /* Opening client fifo for writing  */
      printf("Opening client fifo \'%s\' for writing...", fifocntname);
      fdcnt = open(fifocntname, O_WRONLY);
      if(fdcnt == -1)
	{
	  printf("FAIL!\nError: %s\n", strerror(errno));
	  break;
	}

      /* Sending responce */
      printf("OK\nWritting responce to client [%ld]...", (long)msg.pid);
      bwrite = write(fdcnt, &msg, sizeof(msg));
      if(bwrite == -1)
	{
	  printf("FAIL!\nError: %s\n", strerror(errno));
	  break;
	}
      printf("OK\n");
      close(fdcnt);

    }
  
  
  /* Cleaning up */
  close(fdsrv);
  unlink(fifosrvname);
  
  return 0;
}

/* creat the doctor's response */
void respond (char *s)
{
    char t[80];
    int loc;
    if (strlen(s)<VERYSHORT && strcmp(s,"bye"))  {
       if (find_topic(t))  {
	  snprintf(responce, MESSAGE_BUF_SIZE, "You just said:%s\ntell me more .\n",t);
       }
       else {
          if (!*response[res])  res=0;   /* start over again */
          snprintf (responce, MESSAGE_BUF_SIZE, "%s\n", response[res++]);
       }
       return ;
    }
    if (in_topics(s))  {
       snprintf (responce, MESSAGE_BUF_SIZE, "Stop repeating yourself !\n");
       return ;
    }
    if (strlen(s)>SHORT) assert_topic(s);
    do {
       get_token();
       loc=lookup(token);
       if (loc!=-1)  {
	  snprintf (responce, MESSAGE_BUF_SIZE, "%s\n",trans[loc+1]);
          return ;
       }
    } while (*token);
    /* comment of last resort */
    if (strlen(s)>SHORT)
       snprintf (responce, MESSAGE_BUF_SIZE, "It's seem intersting , tell me more ...\n");
    else snprintf (responce, MESSAGE_BUF_SIZE, "Tell me more ...\n");
}

/* Lookup a keyword in translation table  */
int lookup (char *token)
{
    int t;
    t=0;
    while (*trans[t])  {
          if (!strcmp (trans[t],token))  return t ;
          t++;
    }
    return -1;
}

/* place a topic into the topics database  */
void assert_topic (char *t)
{
    if (head==MAX)  head=0;  /* wrap around */
    strcpy(topics[head],t);
    head++;
}

/* retrieve a topic  */
int find_topic (char *t)
{
    if (tail!=head)  {
       strcpy(t,topics[tail]);
       tail++;
       /* wrap around if necessary */
       if (tail==MAX) tail=0;
       return 1;
    }
    return 0;
}

/* see if in topics queue */
int in_topics (char *s)
{
    int t;
    for (t=0;t<MAX;t++)
        if (!strcmp(s,topics[t]))  return 1;
    return 0;
}

/* return a token from the input stream */
void get_token (void)
{
    char *p;
    p=token;
    /* skip spaces */
    while (*p_pos==' ') p_pos++;

    if (*p_pos=='\0') {  /*is end of input*/
       *p++='\0';
       return;
    }
    if (is_in(*p_pos,".!?")) {
       *p=*p_pos;
       p++,p_pos++;
       *p='\0';
       return ;
    }

    /*read word until*/
    while(*p_pos!=' '&&!is_in(*p_pos,".,;?!")&&*p_pos)  {
         *p=tolower(*p_pos++);
         p++;
    }
    *p='\0';
}

int is_in(char c, char *s)
{
    while(*s) {
       if(c==*s) return 1;
       s++;
    }
    return 0;
}

