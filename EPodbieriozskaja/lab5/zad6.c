#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>


int tab[2][10];
int sum[2];
pthread_t threads[2];
pthread_mutex_t mutex;

void* row_sum(void* arg);

void main(){
    pthread_attr_t attr;
    void* status;
    time_t t;


    pthread_mutex_init(&mutex, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    srand((unsigned) time(&t));
    for (int i=0;i<2;i++){
        for (int j=0;j<10;j++){
            tab[i][j] = rand() % 100;
        }
    }

    for(int i=0; i<2; i++){
        pthread_create(&threads[i], &attr, row_sum, (void*) i);
    }

    pthread_attr_destroy(&attr);
    for(int i=0; i<2; i++){
        pthread_join(threads[i], &status);
    }

    int res=0;
    for (int k = 0; k < 2; k++) {
        res += sum[k];
    }
    printf("Sum is %d", res);

    pthread_mutex_destroy(&mutex);
}

void* row_sum (void* arg){
    int r_sum=0;
    int k=(int) arg;

    for(int i=0; i<10; i++){
        r_sum += tab[k][i];
    }
    pthread_mutex_lock(&mutex);
    sum[k] = r_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit((void *) 0);
}
