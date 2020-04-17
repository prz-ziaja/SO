#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// ----------------------------------------------------------

void *PrintHello(void *arg);
// ----------------------------------------------------------

int main(int argc, char *argv[]){
	int n = 10;	
	pthread_t thread[n];
	for( int i=0; i<n; i++){
	     int rc = pthread_create(&thread[i], NULL, PrintHello, (void *)i);
	     if (rc){
		  printf("Return code: %d\n", rc);
		  exit(-1);
	     }
	}
	for( int i=0; i<n; i++){
		int rcj = pthread_join(thread[i], NULL);
		if (rcj){
		  printf("Return code: %d\n", rcj);
		  exit(-1);
	     	}
	}
	printf("End of the main thread!\n");
     return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
	sleep(1);
     printf("Nr: %ld\n",(intptr_t)arg);
     return NULL;
}