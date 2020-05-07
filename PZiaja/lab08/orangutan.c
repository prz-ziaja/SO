/*
    Wspolna pamiec + semafory
    -------------------------

*/

#include "unix.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define Liczba_krokow 
#define LEWA 12
#define PRAWA 13
#define KLUCZ 22458
#define KLUCZ2 12458
// wszystkie osoby musza miec inny klucz !
// (nr legitymacji ?)

struct KONTA {
  long konto1, konto2;
  int most[5];
  int sem;
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



      c=(konta_t)shmat(id, 0, 0);

      if(c==(konta_t)-1) { perror("pm: shmat; Blad !!!"); exit(1); }

      c->konto1=LEWA;
      c->konto2=PRAWA;
for(int h=0;h<5;h++)
	c->most[h]=0;
c->sem=1;
      while(1) {


if(c->sem!=1){sleep(1); continue;}
int h;
printf("Przejscie lewa -> prawa\n");
printf("Orangutany z lewej: %li Orangutany z prawej: %li\n",c->konto1,c->konto2);
do{
h=0;
int r=rand()%2;

if(r==0||c->konto1==0)
	for(int j=0; j<5; j++){
		if(c->most[j]!=0){
			c->most[j]+=1;
			if(c->most[j]==Liczba_krokow){
				c->konto2+=1;
				c->most[j]=0;
printf("orangutan schodzi z liny\n");
			}	
		}
		if(c->most[j]!=0)
			h=1;
	}
else
	for(int j=0; j<5; j++){
		if(c->most[j]!=0){
			c->most[j]+=1;
			if(c->most[j]==Liczba_krokow){
				c->konto2+=1;
				c->most[j]=0;
printf("orangutan schodzi z liny\n");
			}	
		}
		else if(r>0&&c->konto1>0){
			r-=1;
			c->konto1-=1;
			c->most[j]+=1;
printf("orangutan wchodzi na linę\n");
		}
		
		if(c->most[j]!=0)
			h=1;
	}

}while(h);
c->sem=-1;


      
	if(koniec) break;
      }
    
      i=shmdt(c);
      if(i==-1) perror("pm: shmdt; Blad !!!");
      i=shmdt(c->most);
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


      c=(konta_t)shmat(id, 0, 0);
      if(c==(konta_t)-1) { perror("pp: shmat; Blad !!!"); exit(1); }

      while(1) {


if(c->sem==1){sleep(1); continue;}
	/*long przelew=random();
	c->konto1-=przelew;
	c->konto2+=przelew;*/

int h;
printf("Przejscie prawa ->lewa\n");
printf("Orangutany z lewej: %li Orangutany z prawej: %li\n",c->konto1,c->konto2);
do{
h=0;
int r=rand()%2;
if(r==0||c->konto2==0)
	for(int j=0; j<5; j++){
		if(c->most[j]!=0){
			c->most[j]+=1;
			if(c->most[j]==Liczba_krokow){
				c->konto1+=1;
				c->most[j]=0;
printf("orangutan schodzi z liny\n");
			}	
		}
		if(c->most[j]!=0)
			h=1;
	}
else
	for(int j=0; j<5; j++){
		if(c->most[j]!=0){
			c->most[j]+=1;
			if(c->most[j]==Liczba_krokow){
				c->konto1+=1;
				c->most[j]=0;
printf("orangutan schodzi z liny\n");
			}	
		}
		else if(r>0&&c->konto2>0){
			r-=1;
			c->konto2-=1;
			c->most[j]+=1;
printf("orangutan wchodzi na line\n");
		}
		
		if(c->most[j]!=0)
			h=1;
	}

}while(h);


      c->sem=1;
	if(koniec) break;
      }
    
      i=shmdt(c);
      if(i==-1) perror("pp: shmdt; Blad !!!");
      printf("pp: koniec\n");
      exit(0);
    }

}
