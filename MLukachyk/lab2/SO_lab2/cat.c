////
//// Created by Maryna Lukachyk on 2020-03-24.
////


#include<stdio.h>
#include<unistd.h> /*for STDOUT_FILENO*/
#include<stdlib.h>
#include<fcntl.h> /*for open*/

int main(int argc,char *argv[])
{
    int fd, i, ch;

    for (i = 1; i < argc; i++) {                    /*iterate the loop for total argument count.*/

        fd = open(argv[1],O_RDONLY);            /*open the file in READONLY mode*/

        if(fd < 0) {
            perror("open");
            return -1;
        }

        while(read(fd,&ch,1))                   /*Read one byte at a time*/

            write(STDOUT_FILENO,&ch,1);     /*Write the byte on Stdout*/

        close(fd);
    }

    return 0;}