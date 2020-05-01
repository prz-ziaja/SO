#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ORANGUTANS 20

typedef struct{
  int number_of_orangutans; /* 0-5 */
  int left_fifo[20];         /* if conflict, first in fifo */        
  int right_fifo[20];
  int fifo_priority;        /* if conflict -1-left first 1- right first */
  int direction;            /* -1- from left to right 1-from right to left 0-free*/
} line;
