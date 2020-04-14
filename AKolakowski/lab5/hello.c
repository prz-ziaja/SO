#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
// ----------------------------------------------------------

void *PrintHello(void *arg);
// ----------------------------------------------------------

int main(int argc, char *argv[]){
     int n;

     for(n=1;n<=10;n++)
     {
	     pthread_t thread;
	     int rc = pthread_create(&thread, NULL, PrintHello, (void *)n);
	     if (rc){
		  printf("Return code: %d\n", rc);
		  exit(-1);
	     }
	     sleep(1);
     }
     return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
     printf("My number: %d\n",(int)arg);
     printf("My thread id: %d\n\n",syscall(__NR_gettid));
     return NULL;
}
