#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
    struct termios old,new;
    tcgetattr(STDIN_FILENO,&old);

    new=old;
    new.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&new);
    char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
                 "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
                 "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
    char bufor[6] = "";
    char znak;


    while(read(0,&znak,1) && znak != '\n') {

           strcpy(bufor,kod[znak-'a' ]);

           write(1,bufor, 6);


    }


    tcsetattr(STDIN_FILENO,TCSANOW,&old);
    return 0;
}
