#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE* fp;      
  char c;
  fp = argc == 1 ? stdin : fopen(argv[1], "r");
    if(fp == NULL){
        if(errno == EACCES){          
            perror("ERROR: Błąd dostępu");
        } else if(errno == ENOENT){   
            perror("ERROR: Nie istnieje plik!");
        } else {
            perror("ERROR");
            
        }
	exit(EXIT_FAILURE);
    }
  while ((c = getc(fp)) != EOF) {
    printf("%c", c);
  }

  fclose(fp);
  return 0;
}
