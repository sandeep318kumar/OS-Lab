#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#define MAXCHAIRS 5
#include<unistd.h>
using namespace std;
sem_t customer=0,barber=0,donecutting=0;
pthread_mutex_t lock;
int nowaiting=0;
void * Barber(void *Arg)
{
	while(true)
	{
		sem_wait(&customer);
		cout<<"Barber allocated\n";
		sem_post(&barber);
		cout<<"Barber is cutting hair\n";
		sleep(2);
		sem_post(&donecutting);
		pthread_mutex_lock(&lock);
		nowaiting--;
		pthread_mutex_unlock(&lock);
	}
}
void * Customer(void *)
{
	pthread_mutex_lock(&lock);
	if(nowaiting<MAXCHAIRS)
	{
		nowaiting++;
		cout<<"Customer Sitting\n";
		pthread_mutex_unlock(&lock);
		sem_post(&customer);
		sem_wait(&barber);
		sem_wait(&donecutting);
		cout<<"Customer exiting after getting service \n";
	}
	else
	{
		pthread_mutex_unlock(&lock);
	}
}
int main()
{
	pthread_t Thread[40];
	sem_init(&customer,0,0);
	sem_init(&barber,0,0);
	sem_init(&donecutting,0,0);
	pthread_create(&Thread[0],NULL,Barber,NULL);
	for(int i=1;i<40;i++)
	{
		pthread_create(&Thread[i],NULL,Customer,NULL);
	}
	for(int i=1;i<40;i++)
	{
		pthread_join(Thread[i],NULL);
	}
}
