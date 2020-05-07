/*
    Wspolna pamiec + semafory
    -------------------------

*/

#include "unix.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define Liczba_krokow 5
#define KLUCZ 22456
// wszystkie osoby musza miec inny klucz !
// (nr legitymacji ?)
int most[5]={0};
struct KONTA {
  long konto1, konto2;
};
typedef struct KONTA *konta_t;
konta_t c;

int koniec=0;

void ObslugaSIGINT(int i)
{
  printf("SIGINT: getpid()=%i, konto1+konto2=%li\n",
	 getpid(),
    c->konto1+c->konto2
	 );
  koniec=1;
}

main()
{
  signal(SIGINT,ObslugaSIGINT);

  int pid=fork();
  if(pid!=0)
    { // pm
      printf("pm: poczatek; getpid()=%i\n", getpid());
  
      int i,j;
      printf("pm: uzyskujemy id wspolnej pamieci ...\n");
      int id= shmget(KLUCZ, sizeof(struct KONTA), IPC_CREAT|IPC_EXCL|0666);
      if(id==-1) perror("pm: shmget; Blad !!!");

      printf("pm: uzyskujemy id zbioru semaforow ...\n");
      int id2= semget(KLUCZ, 1, IPC_CREAT|IPC_EXCL|0666);
      if(id2==-1) perror("pm: semget; Blad !!!");
    
      printf("pm: inicjujemy semafory ...\n");
      // podobno to powinno byc zdefiniowane w <sys/sem.h> ???
      union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
      };
      union semun arg;
      arg.val=0;
      j=semctl(id2, 0, SETVAL, arg);
      if(j==-1) perror("pm: semctl; Blad !!!");

      c=(konta_t)shmat(id, 0, 0);

      if(c==(konta_t)-1) { perror("pm: shmat; Blad !!!"); exit(1); }

      c->konto1=12L;
      c->konto2=0;

      while(1) {

	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_op=-1;
	buf.sem_flg=0;
printf("_1_!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	i=semop(id2, &buf, 1);

	if(i==-1) perror("pm: semop(-1); Blad !!!");

/*	long przelew=random();
	c->konto1-=przelew;
	c->konto2+=przelew;*/

int h;
printf("Przejscie lewa -> prawa\n");
do{
h=0;
int r=rand()%2;
if(r==0||c->konto1==0)
	for(int j=0; j<5; j++){
		if(most[j]!=0){
			most[j]+=1;
			if(most[j]==Liczba_krokow){
				c->konto2+=1;
				most[j]=0;
			}	
		}
		if(most[j]!=0)
			h=1;
	}
else
	for(int j=0; j<5; j++){
		if(most[j]!=0){
			most[j]+=1;
			if(most[j]==Liczba_krokow){
				c->konto2+=1;
				most[j]=0;
			}	
		}
		else if(r>0&&c->konto1>0){
			c->konto1-=1;
			most[j]+=1;
		}
		
		if(most[j]!=0)
			h=1;
	}

}while(h);
printf("_1___________________________________________________\n");
	buf.sem_num=0;
	buf.sem_op=+1;
	buf.sem_flg=0;
	i=semop(id2, &buf, 1);
	if(i==-1) perror("pm: semop(+1); Blad !!!");
      
	if(koniec) break;
      }
    
      i=shmdt(c);
      if(i==-1) perror("pm: shmdt; Blad !!!");

      int pid1,status;
      printf("pm: wait() ...\n");
      pid1=wait(&status);
      printf("pm: wait()=%i status=%04X\n", pid1, status);
      printf("pm: wait() ...\n");
      pid1=wait(&status);
      printf("pm: wait()=%i status=%04X\n", pid1, status);

      printf("pm: usuwamy segment pamieci dzielonej\n");
      i=shmctl(id, IPC_RMID, NULL);
      if(i==-1) perror("pm: shmctl(IPC_RMID); Blad !!!");

      printf("pm: usuwamy zbior semaforow\n");
      i=semctl(id2, 0, IPC_RMID, NULL);
      if(i==-1) perror("pm: semctl(IPC_RMID); Blad !!!");

      printf("pm: koniec\n");
    }
////////////////////////////////////////////////////////////////////////////////
  else
    { // pp
      printf("pp: poczatek; getpid()=%i\n", getpid());
      sleep(1);

      int i,j;
      printf("pp: uzyskujemy id wspolnej pamieci ...\n");
      int id= shmget(KLUCZ, 0, 0);
      if(id==-1) perror("pp: shmget; Blad !!!");

      printf("pp: uzyskujemy id zbioru semaforow ...\n");
      int id2= semget(KLUCZ, 0, 0);
      if(id2==-1) perror("pp: semget; Blad !!!");

      c=(konta_t)shmat(id, 0, 0);
      if(c==(konta_t)-1) { perror("pp: shmat; Blad !!!"); exit(1); }

      while(1) {

	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_op=-1;
	buf.sem_flg=0;
	i=semop(id2, &buf, 1);
	if(i==-1) perror("pp: semop(-1); Blad !!!");

	/*long przelew=random();
	c->konto1-=przelew;
	c->konto2+=przelew;*/
printf("_2_!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
int h;
printf("Przejscie prawa ->lewa\n");
do{
h=0;
int r=rand()%2;
if(r==0||c->konto2==0)
	for(int j=0; j<5; j++){
		if(most[j]!=0){
			most[j]+=1;
			if(most[j]==Liczba_krokow){
				c->konto1+=1;
				most[j]=0;
			}	
		}
		if(most[j]!=0)
			h=1;
	}
else
	for(int j=0; j<5; j++){
		if(most[j]!=0){
			most[j]+=1;
			if(most[j]==Liczba_krokow){
				c->konto1+=1;
				most[j]=0;
			}	
		}
		else if(r>0&&c->konto2>0){
			c->konto2-=1;
			most[j]+=1;
		}
		
		if(most[j]!=0)
			h=1;
	}

}while(h);
printf("_2___________________________________________________\n");
	buf.sem_num=0;
	buf.sem_op=+1;
	buf.sem_flg=0;
	i=semop(id2, &buf, 1);
	if(i==-1) perror("pp: semop(+1); Blad !!!");
      
	if(koniec) break;
      }
    
      i=shmdt(c);
      if(i==-1) perror("pp: shmdt; Blad !!!");

      printf("pp: koniec\n");
      exit(0);
    }

}
