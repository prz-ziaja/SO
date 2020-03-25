#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
	int t=open("./test",O_RDONLY);
	struct stat finfo;
	fstat(t,&finfo);
	fprintf(stdout,"%ld\n",finfo.st_size);
	if ((S_IFSOCK & S_IFREG) == S_IFREG) { 
		fprintf(stdout,"ok\n");
	}
}
