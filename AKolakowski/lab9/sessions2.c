#define  _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {

  pid_t pid;
  pid = fork();

  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) {
    printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),getsid(0));
  }
  if (pid == 0) {
    if(setsid() == -1)
    {
      perror("setsid");
      exit(EXIT_FAILURE);
    }
    printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),getsid(0));
  }

  sleep(10); /* w celu obserwacji poleceniem pstree */
  exit(EXIT_SUCCESS);
}
