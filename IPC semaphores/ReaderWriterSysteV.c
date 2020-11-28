#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>
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
	int readercount;
};
void  reader(int semid, struct smph * shm)
{
	printf("Attempting to read\n");
	p.sem_num=0;
	semop(semid,&p,1);
	shm->readercount++;
	if(shm->readercount==1)
	{
		p.sem_num=1;
		semop(semid,&p,1);
	}
	int q=rand()%5;
	printf("Reading\n");
	v.sem_num=0;
	semop(semid,&v,1);
	sleep(q);
	p.sem_num=0;
	semop(semid,&p,1);
	shm->readercount--;
	if(shm->readercount==0)
	{
		v.sem_num=1;
		semop(semid,&v,1);
	}	
	v.sem_num=0;
	semop(semid,&v,1);
}
void writer(int semid, struct smph * shm)
{
	int q=rand()%3;
	printf("Atempting to write\n");
	p.sem_num=1;
	semop(semid,&p,1);
	printf("Writing \n");
	sleep(q);
	v.sem_num=1;
	semop(semid,&v,1);
}	
int main()
{
	struct smph *shm;
	int shmid,semid,key,i;
	shmid=shmget(key,sizeof(struct smph),IPC_CREAT|0660);
	if(shmid==-1)
	perror("Shared Memory fault\n");
	shm=shmat(shmid,NULL,0);
	if(shm== (void *) -1)
		perror("Attachment fault\n");
		shm->readercount=0;
	union semun u;
	semid = semget(KEY, 2, 0666 | IPC_CREAT);
	u.val=1;
	semctl(semid, 0, SETVAL, u); // Lock 	
	semctl(semid,1,SETVAL,u);    // Write LOck
	for(i=0;i<15;i++)
	{
		int pid=fork();
		if(pid==0&&(i%2)==0)
		{
			shm= (struct  smph *) shmat(shmid,NULL,0);
			reader(semid,shm);
			break;
		}
		else if(pid==0&&(i%2)==1)
		{
			shm= (struct  smph *) shmat(shmid,NULL,0);
			writer(semid,shm);
			break;
		}
	}
		
	while(2)
	{
		int r=wait(NULL);
		if(r<0)
		break;
	}	
		
		
	return 0;
}
