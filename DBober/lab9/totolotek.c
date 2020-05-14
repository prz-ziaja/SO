#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void) {

  pid_t pid, sid;
  pid = fork();
  if (pid < 0 || pid > 0) exit(EXIT_FAILURE);

	if(setsid() == -1) exit(EXIT_FAILURE);
	system("umask 0");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	int n=10;
	for(int i=0; i<n; i++){
		system("shuf -i1-49 -n 6 > wyniki_lottot.txt");
		sleep(3);
	}
	return 0;
}
