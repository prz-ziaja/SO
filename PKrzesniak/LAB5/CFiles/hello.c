#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
// ----------------------------------------------------------

void *PrintHello(void *arg);
// ----------------------------------------------------------

int main(int argc, char *argv[]){
	pthread_t threads[10];

	for(int i=0; i<10; i++){


		int rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
		if (rc){
			printf("Return code: %d\n", rc);
			exit(-1);
		}
	}
        printf("End of the main method\n\n");
	sleep(1);
	return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
	printf("Number of loop: %d\n",(int)arg);
	printf("Thread id: %d\n\n",syscall(__NR_gettid));
	return NULL;
}