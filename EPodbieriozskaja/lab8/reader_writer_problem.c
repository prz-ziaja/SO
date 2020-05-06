#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>


#define NR_PROC 16

union semun {
    int             val;            /* value for SETVAL */
    struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
    unsigned short  *array;         /* array for GETALL & SETALL */
};

union semun semval;

struct shdata {
    int readers_number;
    char message[100];

};


void writer(int id, int mux, int shmid)
{
    struct sembuf down = {0,-1,0},
            up = {0,1,0};
    char name[512];
    struct shdata *shm = (struct shdata *)shmat(shmid,NULL,0);
    if(shm==(void *)-1)
        perror("writer shmat");
    if(semop(mux,&down,1)<0)
        perror("writer semop down");
    snprintf(name,512,"Jestem pisarzem nr %d\n",id);
    write(STDOUT_FILENO,name,strlen(name));
    snprintf(shm->message,4096,"Wiadomość od pisarza nr %d\n",id);
    if(semop(mux,&up,1)<0)
        perror("writer semop up");
    if(shmdt(shm)<0)
        perror("writer shmdt");
}

void reader(int id, int mux, int shmid) {
    struct sembuf down0 = {0, -1, 0},
            up0 = {0, 1, 0},
            down1 = {1, -1, 0},
            up1 = {1, 1, 0};

    char name[512];
    struct shdata *shm = (struct shdata *) shmat(shmid, NULL, 0);
    if (shm == (void *) -1)
        perror("reader shmat");
    if (semop(mux, &down1, 1) < 0)
        perror("reader semop 1 down1");
    shm->readers_number++;
    if (shm->readers_number == 1)
        if (semop(mux, &down0, 1) < 0)
            perror("reader semop down0");
    if (semop(mux, &up1, 1) < 0)
        perror("reader semop 1 up1");
    snprintf(name, 512, "Jestem czytelnikiem nr %d\n", id);
    write(STDOUT_FILENO, name, strlen(name));
    write(STDOUT_FILENO, shm->message, sizeof(shm->message));
    if (semop(mux, &down1, 1) < 0)
        perror("reader semop 2 down1");
    shm->readers_number--;
    if (!shm->readers_number)
        if (semop(mux, &up0, 1) < 0)
            perror("reader semop up0");
    if (semop(mux, &up1, 1) < 0)
        perror("reader semop 2 up1");
    if (shmdt(shm) < 0)
        perror("reader shmdt");
}


int main(void) {
    int reader_id = 0, writer_id = 0;
    int mux = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT | IPC_EXCL);
    if (mux < 0)
        perror("semget");
    semval.val = 1;
    if (semctl(mux, 0, SETVAL, semval) < 0)
        perror("semctl 0");
    if (semctl(mux, 1, SETVAL, semval) < 0)
        perror("semctl 1");
    int shmid = shmget(IPC_PRIVATE, 4096, 0600 | IPC_CREAT | IPC_EXCL);
    if (shmid < 0)
        perror("shmget");
    srand(time(NULL));
    int i;
    for (i = 0; i < NR_PROC; i++) {
        int choice = rand() % 2;
        if (choice)
            reader_id++;
        else
            writer_id++;
        int pid = fork();
        if (pid == -1)
            perror("fork");
        if (pid == 0) {
            if (choice)
                reader(reader_id, mux, shmid);
            else
                writer(writer_id, mux, shmid);
            return 0;
        }
    }
    for (i = 0; i < NR_PROC; i++)
        if (wait(NULL) < 0)
            perror("wait");
    if (shmctl(shmid, IPC_RMID, NULL) < 0)
        perror("shmctl");
    if (semctl(mux, 0, IPC_RMID, NULL) < 0)
        perror("semctl IPC_RMID");
    return 0;
}