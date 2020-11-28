#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
pthread_mutex_t lock,wrt;
int readercount=0;
void  *reader(void * v)
{
	printf("Attempting to read\n");
	pthread_mutex_lock(&lock);
	readercount++;
	if(readercount==1)
	{
		pthread_mutex_lock(&wrt);
	}
	int p=rand()%5;
	printf("Reading\n");
	pthread_mutex_unlock(&lock);
	sleep(p);
	pthread_mutex_lock(&lock);
	readercount--;
	if(readercount==0)
	pthread_mutex_unlock(&wrt);	
	pthread_mutex_unlock(&lock);
}
void *writer(void * v)
{
	int p=rand()%3;
	printf("Atempting to write\n");
	pthread_mutex_lock(&wrt);
	printf("Writing \n");
	sleep(p);
	pthread_mutex_unlock(&wrt);
}	
int main()
{
	pthread_t Thread[30];
	pthread_mutex_init(&lock,NULL);
	pthread_mutex_init(&wrt,NULL);
	for(int i=0;i<30;i++)
	{
		pthread_create(&Thread[i],NULL,&reader,NULL);
		pthread_create(&Thread[29-i],NULL,writer,NULL);
	}
	for(int i=0;i<10;i++)
	pthread_join(Thread[i],NULL);
	return 0;
}
