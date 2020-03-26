/*
* copy1.c
* 
* based on: 
*
* Miscellaneous examples for Chap. 2
* AUP2, Chap. 2
*
* modified by WTA
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 512

void copy(char *from, char *to)  /* has a bug */
{
    int fromfd = -1, tofd = -1;
    ssize_t nread;
    char buf[BUFSIZE];
    
    fromfd = open(from, O_RDONLY);
    if(fromfd == -1)
    {
        char messege[BUFSIZE];
        if(errno == EACCES)
        {
            strcpy(messege, "Brak uprawnień do otwarcia pliku ");
            strcat(messege, from);
            fprintf(stderr, messege);
        }
        else if(errno == ENOENT)
        {
            strcpy(messege, "Brak pliku ");
            strcat(messege, from);
            fprintf(stderr, messege);
        }
        else
        {
            fprintf(stderr, strerror(errno));
        }
        return;
    }
    tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
                S_IRUSR | S_IWUSR);
    if(tofd == -1)
    {
        fprintf(stderr, strerror(errno));
        close(fromfd);
        return;
    }
    ssize_t nwrite = 0;
    while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
    {
        if(nread == -1)
        {
            fprintf(stderr, strerror(errno));
            break;
        }
        ssize_t nattempt = nread;
        do
        {
            nwrite = write(tofd, buf, nattempt);
            nattempt -= nwrite;
            if(nwrite == -1)
                nattempt = -1;
        } while (nattempt > 0);
        if(nattempt == -1)
        {
            fprintf(stderr, strerror(errno));
        }
    }
    
    close(fromfd);
    close(tofd);
    return;
}
    
int main(int argc, char **argv){
    if (argc != 3)
    {
        fprintf(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);
        return 1;
    }
    copy(argv[1], argv[2]);
    return 0;
}