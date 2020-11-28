#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define MAXITEMS 10
typedef int item;
item buffer[MAXITEMS];
int in=0;
int out=0;
pthread_mutex_t mv =PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Bufferfull=PTHREAD_COND_INITIALIZER;
pthread_cond_t Bufferempty=PTHREAD_COND_INITIALIZER;
void *producer()
{
	item it;
	while(1)
	{	
		pthread_mutex_lock(&mv);
		it=rand()%100;
		printf("Producing Item:  %d \n",it); 
		if((in+1)%MAXITEMS==out)
		{
			pthread_cond_wait(&Bufferempty,&mv);
		}
		buffer[in]=it;
		in=(in+1)%MAXITEMS;
		pthread_mutex_unlock(&mv);
		pthread_cond_signal(&Bufferfull);
	}
}
void *Consumer()
{
	item it;
	while(1)
	{
		pthread_mutex_lock(&mv);
		if(in==out)
		{
			pthread_cond_wait(&Bufferfull,&mv);
		}
		it=buffer[out];
		printf("Consuming Item : %d \n",it);
		out=(out+1)%MAXITEMS;
		pthread_mutex_unlock(&mv);
		pthread_cond_signal(&Bufferempty);
	}
}
int main()
{
	pthread_t consumertid,producertid;
	pthread_create(&producertid,NULL,producer,NULL);
	pthread_create(&consumertid,NULL,Consumer,NULL);
	pthread_join(producertid,NULL);
	pthread_join(consumertid,NULL);	
	return 0;
}
