#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// ----------------------------------------------------------

void *PrintHello(void *arg);
// ----------------------------------------------------------

int main(int argc, char *argv[]){
    pthread_t thread[10];
    int rc;
    for(int i=0; i<10; i++){
        rc = pthread_create(&thread[i], NULL, PrintHello, (void *)i);
        if (rc){
            printf("Return code: %d\n", rc);
            exit(-1);
        }
    }
    sleep(1);
    return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
    printf("Next boring 'Hello World!' version! %d\n", ((int*)arg));
    return NULL;
}