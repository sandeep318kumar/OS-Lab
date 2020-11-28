/*
Author : Sarvesh Gupta
*/
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/sem.h>
#define KEY 123
#define N 10
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};
struct sembuf p = { 0, -1, SEM_UNDO};  // WAIT
struct sembuf v = { 0, +1, SEM_UNDO};   // SIGNAL
struct smph
{
	int Array[10];
	int in;
	int out;
};
void Producers(int semid, struct smph * shm)
{
	int item=rand()%100;
	p.sem_num=0;
	semop(semid,&p,1);
	p.sem_num=2;
	semop(semid,&p,1);
	shm->Array[shm->in]=item;
	printf("Producing item : %d \n",item);
	shm->in=(shm->in+1)%N;
	v.sem_num=2;
	semop(semid,&v,1);
	v.sem_num=1;
	semop(semid,&v,1);
}
void Consumers(int semid, struct smph * shm)
{
	int item;
	p.sem_num=1;
	semop(semid,&p,1);       //  Wait for empty
	p.sem_num=2;
	semop(semid,&p,1);
	item=shm->Array[shm->out];
	shm->out=(shm->out+1)%N;
	printf("Consuming item : %d \n",item);
	v.sem_num=2;
	semop(semid,&v,1);                                 // Signal for full
	v.sem_num=0;
	semop(semid,&v,1);
	
}
int main()
{
	int shmid,semid,key;
	struct smph * shm;
	shmid=shmget(key,sizeof(struct smph),IPC_CREAT|0660);
	if(shmid==-1)
	perror("Shared Memory fault\n");
	shm=shmat(shmid,NULL,0);
	if(shm== (void *) -1)
		perror("Attachment fault\n");
	shm->in=0;
	shm->out=0;
	union semun u;
	u.val = N;
	semid = semget(KEY, 3, 0666 | IPC_CREAT);
	semctl(semid, 0, SETVAL, u); 
	u.val=0;
	semctl(semid,1,SETVAL,u);
	u.val=1;
	semctl(semid,2,SETVAL,u);
	int pid=fork();
	if(pid==0)
	{
		shm=shmat(shmid,NULL,0);
		while(1)
		Producers(semid,shm);
	}
	else
	{
		//shm=shmat(shmid,NULL,0);
		while(1)
		Consumers(semid,shm);
	}
	return 0;
}
