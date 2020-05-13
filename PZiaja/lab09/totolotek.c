#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
int main(void) {
FILE* f;
  pid_t pid, sid;
  pid = fork();
  sid = getsid(0);
  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) {
	printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
	exit(EXIT_SUCCESS);
  }
  if (pid == 0) {
	setsid();
    printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),getsid(0));
    int fd;
    fd = open("wynik", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    if (dup2(fd, 1) == -1) {
        perror("dup2 failed"); 
        exit(1);
    }


int y=0;
while(y<60){
system("shuf -i1-49 -n 6 > wyniki_lotto");
y+=1;
sleep(5);
}
close(STDOUT_FILENO);
  }
  exit(EXIT_SUCCESS);
}

