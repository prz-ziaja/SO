/*
    Wspolna pamiec
    --------------
    
    --> dwa procesy, "pm" i "pp", dokonuja przelewow
        z konto1 na konto2
	(bez wzajemnego wykluczania)

    --> nacisniecie Ctrl-C powoduje zakonczenie procesow
        + wyswietlenie informacji o stanie kont

    --> jest tworzony proces "pp2", wyswietlajacy (co 1 sekunde)
        sume kont

    --> informacje o segmentach pamieci dzielonej:
        polecenie "ipcs"
*/

#include "unix.h"
#include <sys/shm.h>
#include <sys/ipc.h>

#define KLUCZ 12345
// wszystkie osoby musza miec inny klucz !
// (nr legitymacji ?)

struct KONTA {
  long konto1, konto2;
};

struct KONTA *c;
int koniec=0;

struct sembuf
{
  short sem_num;
  short sem_op;
  short sem_flag;
};

struct sembuf buf;

void podnies(int semid, int semnum)
{
  buf.sem_op = 1;
  buf.sem_num = semnum;
  buf.sem_flag = 0;
  semop(semid, &buf, 1);
}

void opusc(int semid, int semnum)
{
  buf.sem_op = -1;
  buf.sem_num = semnum;
  buf.sem_flag = 0;
  semop(semid, &buf, 1);
}


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
  int semaphore;

  int pid = fork();
  if(pid!=0)
  { // pm
      printf("pm: poczatek; getpid()=%i\n", getpid());

  if(fork()==0)
	{ // pp2
    semaphore = semget(KLUCZ, 1, IPC_CREAT|0600);
	  printf("pp2: poczatek; getpid()=%i\n", getpid());
	  sleep(1);

	  int i,j;
	  printf("pp2: uzyskujemy id wspolnej pamieci ...\n");
	  int id= shmget(KLUCZ, 0, 0);
	  if(id==-1) perror("pp2: shmget; Blad !!!");

	  c=(struct KONTA *)shmat(id, 0, 0666);
	  if(c==(struct KONTA *)-1) { perror("pp2: shmat; Blad !!!"); exit(1); }

	  while(1) {
	    sleep(1);
      opusc(semaphore, 1);
	    printf("pp2: konto1+konto2=%li\n",
          c->konto1+c->konto2
		   );
      podnies(semaphore, 0);
	    if(koniec) break;
	  }

	  i=shmdt(c);
	  if(i==-1) perror("pp2: shmdt; Blad !!!");

	  printf("pp2: koniec\n");
	  exit(0);
	}

      // pm
      int i,j;
      semaphore = semget(KLUCZ, 1, 0600);
      printf("pm: uzyskujemy id wspolnej pamieci ...\n");
      int id= shmget(KLUCZ, sizeof(struct KONTA), IPC_CREAT|IPC_EXCL|0666);
      if(id==-1) perror("pm: shmget; Blad !!!");

      c=(struct KONTA *)shmat(id, 0, 0);
      if(c==(struct KONTA *)-1) { perror("pm: shmat; Blad !!!"); exit(1); }

      c->konto1=1234567L;
      c->konto2=0;

      while(1) {
        //sleep(1);
        opusc(semaphore, 0);
	long przelew=random();
	c->konto1-=przelew;
	c->konto2+=przelew;
        podnies(semaphore, 1);
      
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

      i=shmctl(id, IPC_RMID, NULL);
      if(i==-1) perror("pm: shmctl(IPC_RMID); Blad !!!");
    
      printf("pm: koniec\n");
    } 
    else 
    { // pp
      printf("pp: poczatek; getpid()=%i\n", getpid());
      sleep(1);

      int i,j;
      printf("pp: uzyskujemy id wspolnej pamieci ...\n");
      int id= shmget(KLUCZ, 0, 0);
      if(id==-1) perror("pp: shmget; Blad !!!");

      c=(struct KONTA *)shmat(id, 0, 0);
      if(c==(struct KONTA *)-1) { perror("pp: shmat; Blad !!!"); exit(1); }

      while(1) {
	long przelew=random() % 1000;
	c->konto1-=przelew;
	c->konto2+=przelew;
      
	if(koniec) break;
      }
    
      i=shmdt(c);
      if(i==-1) perror("pp: shmdt; Blad !!!");

      printf("pp: koniec\n");
      exit(0);
    }

}
