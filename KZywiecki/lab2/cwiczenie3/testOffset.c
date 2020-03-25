#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
    char buf[512];
    int n;
    /*
    int f1 = open("file", O_CREAT|O_WRONLY|O_TRUNC);
    int f2 = open("file", O_RDONLY);

    write(f1, "To jest jakis tekst", 20);
    n = read(f2, buf, 20);
    fprintf(stdout, "%s, %d \n", buf, n);
    
    close(f1);
    close(f2);

    int f3 = open("filee", O_RDWR|O_TRUNC|O_CREAT);
    write(f3, "To jest inny niewinny tekst", 27);
    n = read(f3, buf, 27);
    fprintf(stdout, "%s, %d \n", buf, n);
    close(f3);
    */

    memset(buf, (unsigned char)0, 512);
    int f4 = open("file", O_RDWR|O_TRUNC|O_CREAT);
    write(f4, "tekst1 ", 8);
    lseek(f4, 0l, SEEK_SET);
    n = read(f4, buf, 8);
    fprintf(stdout, "%s, %d \n", buf, n);

    memset(buf, (unsigned char)0, 512);
    lseek(f4, -1l, SEEK_END);
    write(f4, "tekst 2 ", 9);
    lseek(f4, 0l, SEEK_SET);
    memset(buf, (unsigned char)0, 512);
    n = read(f4, buf, 512);
    fprintf(stdout, "%s, %d \n", buf, n);
    close(f4);
}
