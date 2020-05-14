#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {

  pid_t pid, sid;
  pid = fork();
  sid = getsid(0);
  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) {
    printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
  }
  if (pid == 0) {
      setsid(); //start new session for child process
      printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
  }
  system("pstree -np"); //create tree of processes
  exit(EXIT_SUCCESS);
}
