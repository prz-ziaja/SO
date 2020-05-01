#include "line.h"

int main(int argc, char* argv[]){
  int shmid, i;
  line* struktura;
  if((shmid = shmget(4444, sizeof(line), IPC_CREAT | 0600)) == -1 ){
    perror("Utworzenie segmentu pamieci wspoldzielonej");
    exit(EXIT_FAILURE);
  }
  
  if((struktura = (line *)shmat(shmid,NULL,0)) == -1){
    perror("Przylaczanie segmentu pamieci wspoldzielonej");
    exit(EXIT_FAILURE);
  }

  struktura->number_of_orangutans = 0;
  for(i=0;i<20;i++){
    struktura->left_fifo[i] = -1;
    struktura->right_fifo[i] = -1;
  }
  struktura->fifo_priority = -1;
  struktura->direction = 0;
  


  return 0;
}
