#include <string.h>
#include <unistd.h>
#include <termios.h>

int main(void) {
  char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
	        "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
	        "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
  char bufor[6] = "";
  char znak;

  struct termios old_tio, new_tio;

  tcgetattr(STDIN_FILENO, &old_tio);

  new_tio = old_tio;

  new_tio.c_lflag &= (~ICANON & ~ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  while(read(0,&znak,1) && znak != '\n') {
    strcpy(bufor,kod[znak-'a']);
    write(1,bufor, 6);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
  
  return 0;
}
