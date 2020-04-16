#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// ----------------------------------------------------------

void *PrintHello(void *arg);
// ----------------------------------------------------------

int main(int argc, char *argv[]){
    pthread_t threads[10];
    int rc;
    for(int i=0; i<10; i++){
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
    }
    for(int i=0; i<10; i++){
        pthread_join(threads[i], NULL);
    }
    printf("End of the main thread!\n");
    return 0;
}
// ----------------------------------------------------------

void *PrintHello(void *arg){
    sleep(1);
    printf("Next boring 'Hello World!' version! %d\n", ((int*)arg));
    return NULL;
}

