#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#define EATING 0
#define HUNGRY 1
#define THINKING 2
#define KEY 123
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};
struct sembuf p = { 0, -1, SEM_UNDO};  // WAIT
struct sembuf v = { 0, +1, SEM_UNDO};   // SIGNAL
struct smph
{
	int State[5];
};
void Initialize(struct smph * SHM )
{
	for(int i=0;i<5;i++)
	{
		SHM->State[i]=THINKING;
	}
}
void test(int i, struct smph * SHM, int semid)
{
	if(SHM->State[i]==HUNGRY&&SHM->State[(i+1)%5]!=EATING&&SHM->State[(i+4)%5]!=EATING)
	{
		v.sem_num=i;
		semop(semid,&v,1);
		//sem_post(&S[i]);
	}
}
void Pickup(int i,struct smph * shm, int semid)
{
	p.sem_num=5;
	semop(semid,&p,1);
	//sem_wait(&mutex);
	shm->State[i]=HUNGRY;
	printf("Philosopher %d is hungry\n", i);
	sleep(1);
	test(i,shm,semid);
	v.sem_num=5;
	semop(semid,&v,1);
	//sem_post(&mutex);
	p.sem_num=i;
	semop(semid,&p,1);
	//sem_wait(&S[i]);
}
void PutDown(int i, struct smph * shm, int semid)
{
	//sem_wait(&mutex);
	p.sem_num=5;
	semop(semid,&p,1);
	shm->State[i]=THINKING;
	test((i+1)%5, shm, semid);
	test((i+4)%5, shm, semid);
	v.sem_num=5;
	semop(semid,&v,1);
	//sem_post(&mutex);
}
void  Philosopher(int i, struct smph * shm, int semid)
{
	while(1)
	{
		printf("Philosopher %d is thinking\n",i);
		sleep(2);
		Pickup(i,shm,semid);
		shm->State[i]=EATING;
		printf("Philosopher %d is eating \n",i);
		sleep(2);
		PutDown(i,shm,semid);
	}
}
int main()
{
	int shmid,semid,key;
	struct smph * shm;
	shmid=shmget(key,sizeof(struct smph),IPC_CREAT|0660);
	if(shmid == -1)
	perror("Shared Memory fault\n");
	shm= (struct smph *)shmat(shmid, NULL, 0);
	if(shm == (void *) - 1)
		perror("Attachment fault\n");
	Initialize(shm);
	union semun u;
	semid = semget(KEY, 6, 0666 | IPC_CREAT);
	u.val = 0;
	for(int i=0;i<5;i++)
	{
		semctl(semid, i, SETVAL, u); 
	}
	u.val = 1;
	semctl(semid, 5, SETVAL, u);
	for(int i=0;i<5;i++)
	{
		if(fork() == 0)
		{
			shm = (struct smph *)shmat(shmid, NULL, 0);
			Philosopher(i, shm, semid);
			break;
		}
	}
	wait(NULL);
}
