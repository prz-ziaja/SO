#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios oldt,newt;
char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
        "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
        "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
char bufor[6] = "";
char znak;

int main(void) 
{
    tcgetattr(STDIN_FILENO,&oldt);
    newt=oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO,TCSANOW,&newt);

    while(read(0,&znak,1) && znak != '\n') 
    {
        strcpy(bufor,kod[znak-'a']);
        write(1,bufor, 6);
    }
    tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
    return 0;
}

