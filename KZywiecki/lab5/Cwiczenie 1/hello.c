    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    // ----------------------------------------------------------
     
    void *PrintHello(void *arg);
    // ----------------------------------------------------------
     
    int main(int argc, char *argv[]){
         pthread_t thread[10];
         int args[10];

         for(int i = 0; i < 10; i++)
         {
              args[i] = i;
               int rc = pthread_create(thread + i, NULL, PrintHello, args + i);
         }
          printf("Dispatched all threads\n");
         for(int i = 0; i < 10; i++)
         {
              int ret = pthread_join(thread[i], NULL);
              if(ret == EINVAL)
              {
                   printf("Thread isn't joinable");
              }
              if(ret == ESRCH)
              {
                   printf("Thread not found");
              }
         }
         printf("End of Main thread\n");
         return 0;
    }
    // ----------------------------------------------------------
     
    void *PrintHello(void *arg){
         sleep(1);
         printf("Moj numer to %d\n", *(int*)(arg));
         return NULL;
    }

