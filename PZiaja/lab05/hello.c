#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define TH 10
// ----------------------------------------------------------

void *PrintHello(void *arg);
// ----------------------------------------------------------

int main(int argc, char *argv[]){
	pthread_t thread[TH];
	int rc;
	for(int i=0; i<TH; i++)
		rc = pthread_create(&thread[i], NULL,PrintHello, (void *) i);
	for(int i=0; i<TH; i++)
		pthread_join(thread[i],NULL);
	printf("End of the main thread!\n");
	return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
     printf("Thread %i.  boring 'Hello World!' version!\n",((int) arg));
     return NULL;
}
