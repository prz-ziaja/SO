#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {

  pid_t pid, sid;
  pid = fork();
  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) {
		sid = getsid(0);
    printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
		sleep(10);
  }
  if (pid == 0) {
		setsid();
		sid = getsid(0);
    printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
		sleep(10);
  }

  exit(EXIT_SUCCESS);
}
