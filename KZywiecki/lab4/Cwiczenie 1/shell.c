#include "funcs.h"
/* -------------------------------------------------------------------------------------- */

int main(void)
{
  int res;
  char cmd[MAXCMD];
  struct cmdlist cmds[MAX_COMMANDS];

  /* setting up the initial values of the structure  */
  /* this should be done every time when new object is created */
  setupnewcommand(&cmds);

  /* Setting up file size limit for this process and for the child processes */
  
  /* main loop of the shell */
  /* this loop proceses command wirtten by user in the following steps: */
  /* 1. Displaying command prompt
     2. Reading command from the std input
     3. Parsing this command
     4. Executing parsed commands
     5. Dealocating used memory
   */
  while(1){
    printprompt();
    if(readcmd(cmd, MAXCMD) == RESERROR) continue;
    res = parsecmd(cmd, MAXCMD, cmds);
    printparsedcmds(cmds);
    int result = -1;
    struct cmdlist* iterator = cmds;
    int previousOp = -1;
    while(iterator != NULL)
    {
      if(previousOp == CONJOR && result == 0)
      {
      }
      else
      {
        result = executecmds(iterator);
        if(result == -2)
        {
          break;
        }
      }
      previousOp = iterator->conjuction;
      iterator = iterator->next;
    }
    dealocate(cmds);
    if(result == -2)
    {
      break;
    }
  }

  return 0;
}
/* -------------------------------------------------------------------------------------- */

/* This function sets up the initial values of the fileds in the cmdlist structure  */
void setupnewcommand(struct cmdlist* __cmd)
{
  if(__cmd == NULL)
    return;
  memset(__cmd, 0, sizeof(struct cmdlist)*MAX_COMMANDS);

  __cmd->next = NULL;           /* in order to know where is the end of the list */
  __cmd->argv = NULL;           /* see realloc(3) */
  __cmd->conjuction = CONJUD;
  __cmd->argc = 0;
}
/* -------------------------------------------------------------------------------------- */

/* This function adds the NULL pointer to the end of the argument list  */
/* For more details see exec(3) */
int setupparsedcommand(struct cmdlist* __cmd)
{
  /* Checking id the argument is not NULL */
  if(__cmd == NULL){
    printf("Null pointer at setupparsedcommand.");
    return RESERROR;
  }

  __cmd->argc++;
  __cmd->argv = (char**)realloc(__cmd->argv, __cmd->argc*sizeof(char*));  /* Adding new pointer  */
  if(__cmd->argv == NULL)                                                 /* Checking is allocation was OK  */
    return RESERROR;
  __cmd->argv[__cmd->argc-1] = NULL;                                      /* Setting NULL poniter  */

  return RESSUCCESS;
}
/* -------------------------------------------------------------------------------------- */

/* 2. Reading command from the std input */
int readcmd(char* __buf, int __bufsize)
{
  /* Reading command from the std input  */
  if(fgets(__buf, __bufsize, stdin) == NULL){
    printf("Error while reading -- try again!");
    return RESERROR;
  }
  
  /* If the std input buffer is not empty */
  /* In this case we did not read all the data from the buffer, 
     so the incomplete command should not be executed
     and that is why we ignore it */
  if(strchr(__buf, '\n') == NULL){
    while(1){
      char c = getchar();
      if(c == '\n')
	break;
      else if((c == EOF) && (ferror(stdin)))
	return RESERROR;
    }
    printf("Line is too long -- the command will be ignored.\nMaximal size is %d.", __bufsize-2);
    return RESERROR;
  }
  return RESSUCCESS;
}
/* -------------------------------------------------------------------------------------- */

/* 3. Parsing this command */
int parsecmd(char* __buf, int __bufsize, struct cmdlist* __head)
{
  char* cmd = __buf;                  /* String that must be parsed  */
  char* word;                         /* String between white characters  */
  struct cmdlist* curr = __head;

  /* Reading next word - read strtok(3)  */
  while((word = strtok(cmd, " \t\n")) != NULL){ 
    if(strcmp(word, "||") == 0)
    {
      curr->conjuction = CONJOR;
      curr->next = curr + 1;
      curr ++;
    }
    else if(strcmp(word, "&&") == 0)
    {
      curr->conjuction = CONJAND;
      curr->next = curr + 1;
      curr ++;
    }
    else
    {
      curr->argc++;
      curr->argv = (char**)realloc(curr->argv, sizeof(char*)*curr->argc);   /* memory reallocation - needed for new argument  */
      if(curr->argv == NULL){
        printf("Error while allocating memory!");
        return RESERROR;
      }
      curr->argv[curr->argc-1] = word;                                      /* Storing new argument in the argument vector in our structure */
      cmd = NULL;
    }
    
  }
  curr->conjuction = CONJUD;
  curr->next = NULL;
  /* Setting up parsed command -- the NULL pointer at the end of the parameters list must added  */
  if(setupparsedcommand(curr) == RESERROR){                               
    printf("Error while setting up parsed command.");
    return RESERROR;
  }
  return RESSUCCESS;
}
/* -------------------------------------------------------------------------------------- */

/* 4. Executing parsed commands */
int executecmds(struct cmdlist* __head)
{
  int f, e;
  struct cmdlist* curr = __head;

  if(strcmp(curr->argv[0], "exit") == 0)
  {
    return -2;
  }

  int status;

    f = fork();
    e = errno;

  if(f == 0){
    execvp(curr->argv[0], curr->argv);
    e = errno;
    printf("Error while executing: %s\n", strerror(e));
    _exit(-1);
  }
  if(f == -1){
    printf("Fork error: %s\n", strerror(e));
    return -1;
  }
  if(f > 0)
  {
    waitpid(f, &status, 0);
  }
  status = status != 0 ? -1 : 0;
  return status;
}
/* -------------------------------------------------------------------------------------- */