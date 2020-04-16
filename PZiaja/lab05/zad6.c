#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int sum0,sum1;
int tab[2][10];
pthread_t threads[2];


void* sum(void* arg);


int main (int argc, char *argv[]){
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	srand(10);
	for (int i=0;i<10;i++){
		tab[0][i]=rand()%13;
		tab[1][i]=rand()%13;
	}
	

	for (int i=0;i<2;i++){
		pthread_create(&threads[i], &attr, sum, (void*)i);
	}

	pthread_attr_destroy(&attr);

	for(int i=0;i<2;i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Wynik: %d + %d = %d\n",sum0, sum1,sum0+sum1);

	return 0;
}


void* sum(void* arg){
	int sum = 0;
	int id = (int) arg;
	for(int i=0;i<10;i++){
		sum += tab[id][i];
	}
	switch(id){
		case 0:
			sum0=sum;
			break;
		case 1:	
			sum1=sum;	
			break;
	}
	pthread_exit((void*) 0);
}
