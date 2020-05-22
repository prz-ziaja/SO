#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
    struct termios old,new;
    tcgetattr(STDIN_FILENO,&old); //gets terminal attributes
    new=old;
    new.c_lflag &= ~(ICANON|ECHO); //echoing is disabled
    tcsetattr(STDIN_FILENO,TCSANOW,&new); //the change occurs immediately

    char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
                 "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
                 "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
    char bufor[6] = "";
    char znak;

    while(read(0,&znak,1) && znak != '\n') {
        strcpy(bufor,kod[znak-'a']);
        write(1,bufor, 6);
    }

    tcsetattr(STDIN_FILENO,TCSANOW,&old); //back to previous terminal attributes

    return 0;
}