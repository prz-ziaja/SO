#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
// ----------------------------------------------------------

#define NUM_THREADS 5
#define TARGET 100
// ----------------------------------------------------------

struct thread_params {
    int target;
    int thread_idx;
};

struct to_clean{
    int* tries;
    int thread_idx;
};
// ----------------------------------------------------------

struct thread_params tp[NUM_THREADS];
pthread_t threads[NUM_THREADS];
pthread_mutex_t mutex;
int tries;

void *search(void *arg);
void cleanup(void *args);
// ----------------------------------------------------------

int main (int argc, char *argv[]){
    int ti;
    int target=TARGET;

    tries = 0;
    pthread_mutex_init(&mutex, NULL);

    printf("Searching for: %d\n", target);

    for (ti=0;ti<NUM_THREADS;ti++){
        tp[ti].target = target;
        tp[ti].thread_idx = ti;
        pthread_create(&threads[ti], NULL, search, (void*) &tp[ti]);
    }

    for (ti=0;ti<NUM_THREADS;ti++){
        pthread_join(threads[ti], NULL);
    }

    printf("Number of all iterations: %d.\n", tries);
    pthread_mutex_destroy(&mutex);
    return 0;
}
// ----------------------------------------------------------

void *search(void *arg){
    int threadIdx = 0; // indeks watku
    int toFind = 0;   // wartosc do odszukania
    int ti = 0;
    int rnd;
    int triesThread = 0;
    struct to_clean toClean;
    struct thread_params a = *((struct thread_params*) arg);

    toFind = a.target;
    threadIdx = a.thread_idx;
    pthread_t tid = pthread_self();
    srand(tid);

    toClean.thread_idx = threadIdx;
    toClean.tries = &triesThread;
    pthread_cleanup_push(cleanup, (void*) &toClean); //okreslamy funkcje wykonywana w momecie smierci watka
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);//The thread is cancelable.  This is the default cancelability state
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);//A cancellation request is deferred until the thread next calls a function that is a cancellation point

    while(1){
        while(pthread_mutex_trylock(&mutex) == EBUSY){ //If the mutex is currently locked by another thread => returns an error EBUSY
            pthread_testcancel(); //creates a cancellation point
        }
        tries++;
        triesThread++;
        pthread_mutex_unlock(&mutex);

        rnd = (int)(rand()%1000);
        if (toFind == rnd) {
            while(pthread_mutex_trylock(&mutex) == EBUSY){
                pthread_testcancel();
            }
            printf("Number found by %d!\n", threadIdx);
            for(ti=0;ti<NUM_THREADS;ti++){
                if(ti == threadIdx)
                    continue;
                pthread_cancel(threads[ti]);
            }
            sleep(1);
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    pthread_cleanup_pop(1);
    return((void *)0);
}

void cleanup(void *args){
    struct to_clean a = *((struct to_clean*) args);
    printf("Watek %d probowal %d razy \n", a.thread_idx, *a.tries);
}

