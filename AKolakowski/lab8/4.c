#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>

#define NUM_READERS 10
#define NUM_WRITERS 3
#define SEM_KEY 123456789
#define SEM_NUM 3
#define INCREMENT 1
#define DECREMENT -1
#define IN 0
#define MX 1
#define WRT 2

/* na bazie https://arxiv.org/pdf/1309.4507.pdf */

void initSem(int shr_sem)
{
	int i;

	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short int *array;
		struct seminfo *__buf;
	};

	union semun arg;
	arg.val=1;

	for(i=0;i<SEM_NUM;i++)
		semctl(shr_sem, i, SETVAL, arg);	
}

void editSem(int shr_sem, int sem_num, int modifier)
{
	struct sembuf buf;

	buf.sem_num=sem_num;
	buf.sem_op=modifier;
	buf.sem_flg=0;

	semop(shr_sem, &buf, 1);
}

int main() 
{ 
	int i, shr_mem, shr_sem;
	int *shr_val, *ctr;

	shr_mem = shmget(SEM_KEY, sizeof(int)*2, IPC_CREAT|IPC_EXCL|0777);
	if(shr_mem == -1)
	{
		perror("shmget");
		return 1;
	}

	shr_sem = semget(SEM_KEY, SEM_NUM, IPC_CREAT|IPC_EXCL|0777);
	if(shr_sem == -1)
	{
		perror("semget");
		return 1;
	}
	initSem(shr_sem);

	shr_val = (int*)shmat(shr_mem,NULL,0);
	ctr = (int*)shmat(shr_mem,NULL,0)+1;
	*ctr = 0;

	for(i=0;i<NUM_READERS;i++)
	{ 
		if(fork() == 0) /* READER */
		{ 
			printf("[reader] pid %d start\n",getpid()); 

			editSem(shr_sem,IN,DECREMENT);
			editSem(shr_sem,MX,DECREMENT);

			if(++(*ctr) == 1)
				editSem(shr_sem,WRT,DECREMENT);

			editSem(shr_sem,MX,INCREMENT);
			editSem(shr_sem,IN,INCREMENT);

			printf("[reader] pid %d got value: %d\n",getpid(),*shr_val);

			editSem(shr_sem,MX,DECREMENT);

			if(--(*ctr) == 0)
				editSem(shr_sem,WRT,INCREMENT);

			editSem(shr_sem,MX,INCREMENT);

			shmdt(shr_val);

			return 0;
		} 
	}

	for(i=0;i<NUM_WRITERS;i++)
	{ 
		if(fork() == 0) /* WRITER */
		{ 
			printf("[writer] pid %d start\n",getpid()); 

			editSem(shr_sem,IN,DECREMENT);
			editSem(shr_sem,WRT,DECREMENT);

			srand(getpid());
			*shr_val = rand()%1000;
			printf("[writer] pid %d set value: %d\n",getpid(),*shr_val);

			editSem(shr_sem,WRT,INCREMENT);
			editSem(shr_sem,IN,INCREMENT);

			shmdt(shr_val);

			return 0;
		} 
	}

	for(i=0;i<NUM_READERS+NUM_WRITERS;i++)
		wait(NULL);

	shmdt(shr_val);
	shmctl(shr_mem, IPC_RMID, NULL);
	semctl(shr_sem, -1, IPC_RMID, NULL);

	return 0;
} 

