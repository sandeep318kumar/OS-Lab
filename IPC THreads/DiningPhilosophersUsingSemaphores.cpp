#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<iostream>
#define EATING 2
#define THINKING 0
#define HUNGRY 1
using namespace std;
int State[5];
sem_t S[5];
sem_t mutex;
int TimesEaten[5];
void test(int i)
{
	if(State[i]==HUNGRY&&State[(i+1)%5]!=EATING&&State[(i+4)%5]!=EATING)
	{
		sem_post(&S[i]);
	}
}
void Pickup(int i)
{
	sem_wait(&mutex);
	State[i]=HUNGRY;
	cout<<"Philosopher "<<i<<" is hungry\n";
	sleep(1);
	test(i);
	sem_post(&mutex);
	sem_wait(&S[i]);
}
void PutDown(int i)
{
	sem_wait(&mutex);
	State[i]=THINKING;
	test((i+1)%5);
	test((i+4)%5);
	sem_post(&mutex);
}
void * Philosopher(void * arg)
{
	int i= *((int *) arg);
	while(1)
	{
		cout<<"Philosopher "<<i<<" is thinking\n";
		sleep(2);
		Pickup(i);
		State[i]=EATING;
		TimesEaten[i]++;
		cout<<"Philosopher "<<i<<" is eating for the"<<TimesEaten[i]<<"th time\n";
		sleep(2);
		PutDown(i);
	}
}
int main()
{
	int Phil[5]={0,1,2,3,4};
	pthread_t T[5];
	for(int i=0;i<5;i++)
	{
		State[i]=THINKING;
		TimesEaten[i]=0;
		sem_init(&S[i],0,0);
	}
	sem_init(&mutex,0,1);
	for(int i=0;i<5;i++)
	{
		pthread_create(&T[i],NULL,Philosopher,&Phil[i]);
	}
	for(int i=0;i<5;i++)
	{
		pthread_join(T[i],NULL);
	}
}
