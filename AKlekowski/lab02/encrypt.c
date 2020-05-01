/*
 *  Napisz program szyfrujący plik zadany jako parametr wywołania wg następujących zasad:

    Pierwszy znak jest przepisany z oryginalnego do zaszyfrowanego pliku
    Drugi znak jest liczbą n wylosowaną z przedziału <1;9>
    Kolejne n znaków jest losowane
    Cykl się powtarza (czyli kolejny znak jest z pliku oryginalnego, dalej liczba, dalej losowe znaki, potem powtórka, …) aż do końca pliku oryginalnego.

 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFSIZE 512

void encrypt(char *input_file, char *output_file) {
    int input_fd = open(input_file, O_RDONLY);
    if(input_fd == -1) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(output_fd == -1) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    int nread;
    char buf[BUFSIZE];
    while ((nread = read(input_fd, buf, sizeof(buf))) > 0) {
        if(nread == -1) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }

        for(int i=0; i<nread; i++) {
            int nchar = rand() % 10;
            char *crypted_char = malloc(2+nchar);
            crypted_char[0] = buf[i];
            crypted_char[1] = nchar+48;
            for(int j=2; j<nchar+2; j++) {
                crypted_char[j] = (char)((rand() % 30) + 65);
            }

            int wr_error = write(output_fd, crypted_char, 2+nchar);
            if (wr_error == -1) {
                perror("ERROR");
                exit(EXIT_FAILURE);
            }
        }
    }

    if(close(input_fd) == -1) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    if(close(output_fd) == -1) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    time_t t;
    srand((unsigned) time(&t));

    if(argc == 2) {
        encrypt(argv[1], "encrypted.txt");
    } else if(argc == 4) {
        encrypt(argv[1], argv[3]);
    } else {
        printf("encrypt: invalid option\n");
        exit(EXIT_FAILURE);
    }
}
