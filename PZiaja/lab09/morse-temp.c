#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
struct termios oldtio,newtio;
tcgetattr(STDIN_FILENO,&oldtio);
newtio=oldtio;
newtio.c_lflag=0;
tcsetattr(STDIN_FILENO,TCSANOW,&newtio);
  char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
	        "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
	        "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
  char bufor[6] = "";
  char znak;

  while(read(0,&znak,1) && znak != '\n') {
    strcpy(bufor,kod[znak-'a']);
    write(1,bufor, 6);
  }
tcsetattr(STDIN_FILENO,TCSANOW,&oldtio);
  return 0;
}

