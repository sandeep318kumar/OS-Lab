#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<pthread.h>
#define MAXCHAIRS 5
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#define CUSTOMER 1
#define BARBER 0
#define DONECUTTING 2
#define LOCK 3
#define KEY 123
struct smph
{
	int nowaiting;
};
// Customer 0
// Barber 1
// Done cutting 2
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};
void Post(int semid, int sid)
{
	struct sembuf v = { 0, +1,0};   // SIGNAL
	v.sem_num=sid;
	semop(semid,&v,1);
}
void Wait(int semid, int sid)
{
	struct sembuf p = { 0, -1,0};  // WAIT
	p.sem_num=sid;
	semop(semid,&p,1);
}
void  Barber(int semid, struct smph * shm)
{
	while(1)
	{
		Wait(semid,CUSTOMER);
		printf("Barber allocated\n");
		Post(semid,BARBER);
		printf("Barber is cutting hair\n");
		sleep(2);
		Post(semid, DONECUTTING);
		Wait(semid, LOCK);
		shm->nowaiting--;
		Post(semid,LOCK);
	}
}
void Customer(int semid, struct smph * shm)
{
	Wait(semid,LOCK);
	if(shm->nowaiting<MAXCHAIRS)
	{
		shm->nowaiting++;
		printf("Customer Sitting\n");
		Post(semid,LOCK);
		Post(semid,CUSTOMER);
		Wait(semid,BARBER);
		Wait(semid,DONECUTTING);
		printf("Customer exiting after getting service \n");
	}
	else
	{
	        printf("Customer Left\n");
		Post(semid,LOCK);
	}
}
int main()
{
	int shmid,semid,key,i;
	struct smph * shm;
	shmid=shmget(key,sizeof(struct smph),IPC_CREAT|0660);
	if(shmid==-1)
	perror("Shared Memory fault\n");
	shm= (struct smph *)shmat(shmid,NULL,0);
	if(shm== (void *) -1)
		perror("Attachment fault\n");
	shm->nowaiting=0;
	union semun u;
	semid = semget(KEY, 4, 0660 | IPC_CREAT);
	u.val=0;
	for(int i=0;i<3;i++)
	{
		semctl(semid, i, SETVAL, u); 
	}
	u.val=1;
	semctl(semid,3,SETVAL,u);
	for(i=0;i<20;i++)
	{
		int pid=fork();
		if(pid==0&&i==0)
		{
			//semid = semget(KEY, 4, 0666 | IPC_CREAT);
			shm= (struct smph *)shmat(shmid,NULL,0);
			Barber(semid, shm);
			exit(0);
		}
		else if(pid==0&&i!=0)
		{
			//semid = semget(KEY, 4, 0666 | IPC_CREAT);
			shm= (struct smph *)shmat(shmid,NULL,0);
			Customer(semid, shm);
			exit(0);
		}
	}
	while(1)
	{
		int y=wait(NULL);
		if(y<0)
		break;
	}
}
