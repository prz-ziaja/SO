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
     pthread_t thread[10];

     for(n=1;n<=10;n++)
     {
	     int rc = pthread_create(&thread[n-1], NULL, PrintHello, (void *)n);
	     if (rc){
		  printf("Return code: %d\n", rc);
		  exit(-1);
	     }
     }

     for(n=1;n<=10;n++)
     {
         int res = pthread_join(thread[n-1],NULL);
         if (res){
		  printf("Error: %d\n", res);
		  exit(-1);
	     }
     }


     printf("End of the main thread!\n");
     return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
     sleep(1);
     printf("My number: %d\n",(int)arg);
     printf("My thread id: %d\n\n",syscall(__NR_gettid));
     return NULL;
}
