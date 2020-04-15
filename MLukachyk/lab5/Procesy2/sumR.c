//
// Created by Maryna Lukachyk on 2020-04-15.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define NUM_ROWS 2
#define NUM_COLS 4
void * sumRow(void * arg);
pthread_t threads[NUM_ROWS];
int sum = 0;
pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
    int array[NUM_ROWS][NUM_COLS], i, j;
    pthread_mutex_init(&mutex, NULL);
    srand(time(0));
    for (i = 0; i < NUM_ROWS; i++) {
        for (j = 0; j < NUM_COLS; j++) {
            array[i][j] = rand() % 10;
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }


    for (i = 0; i < NUM_ROWS; i++) {
        pthread_create(&threads[i], NULL, sumRow, (void *) array[i]);
    }

    for (i = 0; i < NUM_ROWS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", sum);

    pthread_mutex_destroy(&mutex);
    return 0;
}

void * sumRow(void * arg) {
    int * a = (int *) arg;
    int sumTemp = 0, i = 0;
    for (i = 0; i < NUM_COLS; i++) {
        sumTemp += a[i];
    }
    pthread_mutex_lock(&mutex);
    sum += sumTemp ;
    pthread_mutex_unlock(&mutex);
    return ((void *) 0);
}