#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define INTERVAL 10

static const char *TMP_DIR = "/tmp";

static inline void close_std_desc()
{
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char const *argv[])
{
    pid_t sid;
    switch (fork())
    {
    case -1:
        perror("Error forking");
        exit(EXIT_FAILURE);
        break;
    case 0:
        //child process
        if ((sid = setsid()) == 0)
        {
            perror("Error setting session id");
            exit(EXIT_FAILURE);
        }

        close_std_desc();
        chdir(TMP_DIR);
        umask(0);

        for (int i = 0; i < 10; i++)
        {
            system("shuf -i1-49 -n 6 > wyniki_lotto");
            sleep(INTERVAL);
        }

        exit(EXIT_SUCCESS);
    default:
        //parent
        exit(EXIT_SUCCESS);
    }
    return 0;
}
