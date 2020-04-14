#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NROWS 2
#define NCOLS 10

pthread_t threads[NROWS];
int tab[NROWS][NCOLS];
long result[NCOLS];

void* calc(void* arg);

int main (int argc, char *argv[]){

	int i,j;
	long sum = 0, sum_t = 0;
	srand(time(NULL));

	for(i=0;i<NROWS;i++)
	{
		for(j=0;j<NCOLS;j++)
		{
			tab[i][j]=(int)(rand()%1000);
			sum+=tab[i][j];
		}
	}

     for(i=0;i<NROWS;i++){
          pthread_create(&threads[i],NULL,calc,(void*)i);
     }


     for(i=0;i<NROWS;i++) {
          pthread_join(threads[i], NULL);
     }

     printf ("Correct result is: %ld \n", sum);

     for(i=0;i<NCOLS;i++)
	sum_t+=result[i];

     printf ("Sum from threads is: %ld \n", sum_t);

     return 0;
}

void* calc(void* arg)
{
     long mysum = 0;
     int i=arg;
     int j;

     for (j=0;j<NCOLS;j++){
          mysum += tab[i][j];
     }

     result[i]=mysum;

     printf("Sum from thread %d = %ld\n",i,mysum);

     pthread_exit((void*) 0);
}
