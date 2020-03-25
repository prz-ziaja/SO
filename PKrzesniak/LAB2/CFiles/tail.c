#define _XOPEN_SOURCE 500

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <zconf.h>
#include <errno.h>

//Ustawiamy tail i end zeby wskazywał na koniec pliku, w pierszej pętli cofamy się do 10 linijek wstecz
// albo natrafimy na początek pliku. Później wypisujemy od moementu zatrzymania do końca pliku.  
void tail(char *path)
{
	char c;
	int fd, ncount;
	off_t tail, end;
 
	fd = open(path, O_RDONLY);
	tail = end = lseek(fd, 1, SEEK_END);
  ncount = 0;

  while (ncount <= 10 && tail > 0) {
      if (pread(fd, &c, 1, tail - 1) == -1) {
          perror("ERROR!!!");
          exit(EXIT_FAILURE);
      }

      if (c == '\n') {
          ncount++;
          if (ncount == 11) { //usuwamy puste spację znak '\n' 
              tail++;
          }
      }

      tail--;
  }

  while (tail < end) {
      if (pread(fd, &c, 1, tail) == -1) {
          perror("Fail");
          exit(EXIT_FAILURE);
      }

      putchar(c);
      tail++;
  }
	
	close(fd);
	return;
}

int main(int argc, char* argv[]) {
    tail(argv[1]);
    
    return 0;
}
