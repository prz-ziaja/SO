#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define WRTS_NUM 2
#define RDS_NUM 3
#define ITERS_NUM 3

union semun {
    int             val;            
    struct semid_ds *buf;           
    unsigned short  *array;        
};

union semun semval;

struct library {
    int readers_num;
    char book[100];

};

void writing(){
	for(int i=0; i<3; i++){
		sleep(1);
		printf("... ");
	}
	printf("\n");
}

void writer(int id, int semid, int shmid)
{
	struct sembuf semON, semOFF;
	semON.sem_num=0;
	semON.sem_op=-1;
	semON.sem_flg=0;
	semOFF.sem_num=0;
	semOFF.sem_op=1;
	semOFF.sem_flg=0;
    char info[512];
    struct library *lib = (struct library *)shmat(shmid,NULL,0);
    if(lib==(void *)-1)
        perror("ERROR:writer shmat");

    semop(semid, &semON, 1);

    snprintf(info, 512, "Pisze pisarz %d\n", id);
    printf(info);
	writing();
	snprintf(info, 4096, lib->book);
    snprintf(lib->book,4096, "%s%d, ", info, id);

    semop(semid, &semOFF,1);

    if(shmdt(lib)<0)
        perror("ERROR:writer shmdt");
}

void reader(int id, int semid, int shmid) {
	struct sembuf semON1, semOFF1, semON2, semOFF2;
	semON1.sem_num=0;
	semON1.sem_op=-1;
	semON1.sem_flg=0;
	semOFF1.sem_num=0;
	semOFF1.sem_op=1;
	semOFF1.sem_flg=0;
	semON2.sem_num=1;
	semON2.sem_op=-1;
	semON2.sem_flg=0;
	semOFF2.sem_num=1;
	semOFF2.sem_op=1;
	semOFF2.sem_flg=0;
    char info[512];

    struct library *lib = (struct library *) shmat(shmid, NULL, 0);
    if (lib == (void *) -1)
        perror("ERROR:reader shmat");

  	semop(semid, &semON2, 1);

    lib->readers_num++;

    if (lib->readers_num == 1)
        semop(semid, &semON1, 1);
    semop(semid, &semOFF2, 1);

    snprintf(info, 512, "Czyta czytelnik %d\n    ", id);
    printf(info);
    printf(lib->book);
	printf("\n");

    semop(semid, &semON2, 1);

    lib->readers_num --;
    if (!lib->readers_num)
        semop(semid, &semOFF1, 1);

    semop(semid, &semOFF2, 1);

    if (shmdt(lib) < 0)
        perror("ERROR:reader shmdt");
}


int main(int argc, char* argv[]){

    int semid = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0)
        perror("semget");

    semval.val = 1;
    if (semctl(semid, 0, SETVAL, semval) < 0)
        perror("ERROR:semctl");
    if (semctl(semid, 1, SETVAL, semval) < 0)
        perror("ERROR:semctl");

    int shmid = shmget(IPC_PRIVATE, 4096, 0600 | IPC_CREAT | IPC_EXCL);
	int i;
    if (shmid < 0)
        perror("shmget");
    int pid = fork();
    if(pid == 0){
		int writerid, this_writerid=0;
		for(i=0;i < WRTS_NUM-1;i++){
				writerid = fork();
				if(writerid == 0)
					break;
				this_writerid += 1;
			}
		for(i=0;i < ITERS_NUM;i++){
			writer(this_writerid,semid, shmid);	
		}
		if(writerid == 0)
			return 0;
	}
	sleep(2);
	if(pid != 0){
			int readerid, this_readerid=0;
			for(i=0;i < RDS_NUM-1;i++){
				readerid = fork();
				if(readerid == 0)
					break;
				this_readerid += 1;
			}
			for(i=0;i < ITERS_NUM;i++){
					sleep(1);
					reader(this_readerid, semid, shmid);	
				}
			return 0;
	}
	sleep(1);
	wait(NULL);
    if (shmctl(shmid, IPC_RMID, NULL) < 0)
        perror("shmctl");
    if (semctl(semid, 0, IPC_RMID, NULL) < 0)
        perror("semctl IPC_RMID");
    return 0;
}
