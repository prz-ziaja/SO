#include "line.h"
#include <stdlib.h>

int main(int argc, char* argv[]){
  
  int shmid;
  int side = -1;
  line* struktura;
  srand(time(0));

  if((shmid = shmget(4444, sizeof(line), IPC_CREAT | 0600)) == -1 ){
    perror("Utworzenie segmentu pamieci wspoldzielonej");
    exit(EXIT_FAILURE);
  }
 
  if((struktura = (line *)shmat(shmid,NULL,0)) == -1){
    perror("Przylaczanie segmentu pamieci wspoldzielonej");
    exit(EXIT_FAILURE);
  }
printf("\n");

  while(1){   
    sleep(rand()%2);
    
    if(struktura->number_of_orangutans < 2 && ( struktura->direction == side || struktura->direction == 0)){
      struktura->number_of_orangutans++;
      struktura->direction = side;
      printf("\nOrangutan %s idzie na stronę %d", argv[1], side*-1);
      sleep(2);    
      struktura->number_of_orangutans--;
      if (struktura->number_of_orangutans == 0)
	struktura->direction = 0;
      side *= -1;
      printf("\nOrangutan %s jest na stronie %d", argv[1], side);
    }
    else if( struktura->number_of_orangutans == 2)
      printf("\nOrangutan %s czeka bo nie ma miejsca na linie", argv[1]);
    else
      printf("\nOrangutan %s czeka bo inny orangutani idzie w jego strone", argv[1]);
  }
  
  

  return 0;
}
