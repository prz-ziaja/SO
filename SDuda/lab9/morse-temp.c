#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
	        "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
	        "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
  char bufor[6] = "";
  char znak;


	struct termios term1;
	struct termios term2;
	if(tcgetattr(STDIN_FILENO, &term1) < 0){
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &term1);
		exit(EXIT_FAILURE);	
	}
	term2= term1;
	term2.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term2);
  while(read(0,&znak,1) && znak != '\n') {
    strcpy(bufor,kod[znak-'a']);
    write(1,bufor, 6);
  }
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &term1);
  return 0;
}
