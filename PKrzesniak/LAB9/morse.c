#include <string.h>
#include <unistd.h>
#include <termios.h>

#define EOL '\n'
#define CLEANUP tcsetattr(STDIN_FILENO, TCSAFLUSH, &init_tp);

static struct termios init_tp;

static inline void parse_chars(char **code)
{
    char bufor[6] = "";
    char znak;

    while (read(0, &znak, 1) && znak != EOL)
    {
        strcpy(bufor, code[znak - 'a']);
        write(1, bufor, 6);
    }
}

int main(void)
{
    char *code[] = {".- ", "-... ", "-.-. ", "-.. ", ". ", "..-. ", "--. ", ".... ", ".. ", ".--- ",
                    "-.- ", ".-.. ", "-- ", "-. ", "--- ", ".--. ", "--.- ", ".-. ", "... ", "- ",
                    "..- ", "...- ", ".-- ", "-..- ", "-.-- ", "--.. "};

    struct termios tp;

    tcgetattr(STDIN_FILENO, &init_tp); //getting current attributes
    tcgetattr(STDIN_FILENO, &tp);

    tp.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
    tp.c_iflag &= ~(INLCR | ICRNL | ISTRIP | INPCK | IXON | BRKINT);
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp);

    parse_chars(code);

    CLEANUP

    return 0;
}

