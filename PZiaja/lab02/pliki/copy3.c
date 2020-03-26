#include <stdio.h>
void copy3(char *from, char *to)
{
	FILE *stfrom, *stto;
	int c;
	
	if ((stfrom = fopen(from, "r") ) == NULL){}
	if(( stto = fopen(to, "w") ) == NULL) {}
	while ((c = getc(stfrom)) != EOF)
		putc(c, stto);
	fclose(stfrom);
	fclose(stto);
	return;
	
}

int main(int argc, char** argv){
	copy3(argv[1], argv[2]);
	return 0;
}