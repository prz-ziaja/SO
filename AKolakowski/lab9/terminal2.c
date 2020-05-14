#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("ttyname(STDIN_FILENO): %s\n", ttyname(STDIN_FILENO));
	return 0;
}
