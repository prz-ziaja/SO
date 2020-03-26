#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int cat(FILE *fdsc){
    char c;

    c = fgetc(fdsc);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fdsc);
    }

    fclose(fdsc);
}


int main(int argc, char *argv[]){
    FILE *fd;
    int i;
    
    if (argc == 1){
        fd = stdin;
        cat(fd);
    } else {
        for (i = 1; i < argc; i++) {
            fd = fopen(argv[i], "r");
            if (fd == NULL) {
                printf("Cannot open file \n");
                exit(0);
            }
            cat(fd);
        }

    }
}

