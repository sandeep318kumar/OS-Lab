#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<ucontext.h>
#include<sys/time.h>
#include<sys/types.h>
#include<stdlib.h>
#define NOTCREATED 0
#define READY 1
#define RUNNING 2
#define WAITING 3	
#define MAXTHREADS 1000
typedef void (*f_ptr)( );
const int MEM=64000;
struct itimerval timer;
int threadrunning=0;
int State[MAXTHREADS]={NOTCREATED};
int incs=0;
int inmain=1;
ucontext_t Main;
int done=0;
int qsize=0;
f_ptr Array[MAXTHREADS]={NULL};
struct readyqueuenode
{
	int tid;
	ucontext_t Con;
	struct readyqueuenode * next;
};
struct thread_mutex
{
	int val;
	int nowaiting;
};
struct readyqueuenode * curr,*head,*tail;
void thread_mutex_init(struct thread_mutex * mutex)
{
	mutex->val=1;
	mutex->nowaiting=0;
}
void thread_mutex_lock(struct thread_mutex * mutex)
{
	incs=1;
	ucontext_t Temp,Temp2;
	getcontext(&Temp);
	if(mutex->val==1)
	{
		mutex->val--;
		if(mutex->nowaiting>0)
		mutex->nowaiting--;
	}
	else if(mutex->val==0)
	{
		mutex->nowaiting++;
		if(curr->next==NULL)
		{
			curr->Con=Temp;
			State[curr->tid]=WAITING;
			inmain=1;
			incs=0;
			swapcontext(&Temp2,&Main);
		}
		else
		{
			curr->Con=Temp;
			State[curr->tid]=WAITING;
			State[curr->next->tid]=RUNNING;
			incs=0;
			swapcontext(&Temp2,&(curr->next->Con));
		}
	}
	incs=0;
}
void thread_mutex_unlock(struct thread_mutex * mutex)
{
	incs=1;
	mutex->val++;
	incs=0;
}
struct thread_mutex mutex;
void Function1()
{
	int i=0,j=0;
	for(i=0;i<5;i++)
	{
		printf("Counting  %d in function 1\n",i);
		//sleep(2);
		for(j=0;j<0x2fffffff;j++);
	}
	done=1;
}
void Function2()
{
	int i=0,j=0;
	//thread_mutex_init(&mutex);
	thread_mutex_lock(&mutex);
	printf("Suraj is creating critical situations for all\n");
	printf("No one can disturb him!!!\n");
	for(j=0;j<0x2fffffff;j++);
	printf("Suraj will ace!\n");
	thread_mutex_unlock(&mutex);
}
void Function3()
{
	int i=0,j=0;
	//thread_mutex_init(&mutex);
	thread_mutex_lock(&mutex);
	printf("We are always in critical situation\n");
	printf("Suraj disturbs us!!!\n");
	for(j=0;j<0x2fffffff;j++);
	printf("We should not ace!\n");
	thread_mutex_unlock(&mutex);
	//done=1;
}
void Scheduler(int signum)
{
	printf("Scheduling\n");
	if(qsize==0||incs==1)
	{
		return;
	}
	if(inmain)
	{
		struct readyqueuenode * RT;
		RT=head;
		if(RT==NULL)
		return;
		else
		{
			getcontext(&Main);
			curr=RT;
			inmain=0;
			State[curr->tid]=RUNNING;
			threadrunning=curr->tid;
			swapcontext(&Main,&curr->Con);
		}
	}
	else
	{
		struct readyqueuenode * RT,* PQ;
		RT=curr->next;
		if(RT==NULL)
		{
			inmain=1;
			State[curr->tid]=WAITING;
			swapcontext(&curr->Con,&Main);
		}
		else
		{
			State[curr->tid]=WAITING;
			//curr=RT;
			PQ=curr;
			State[RT->tid]=RUNNING;
			curr=RT;
			threadrunning=curr->tid;
			swapcontext(&PQ->Con,&RT->Con);
		}
	}
}
void Handler(int signum)
{
	printf("Helloworld\n");
}
void Initialize()
{
 	signal(SIGVTALRM,Scheduler);
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 250000;
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 250000;
 	if(setitimer(ITIMER_VIRTUAL,&timer,NULL)<0) perror("Timerseterror\n");
}
void Wrapper(int tid)
{
	Array[tid]();
	State[tid]=NOTCREATED;
	incs=1;
	struct readyqueuenode * temp;
	if(head->tid==tid)
	{
		temp=head;
		if(head==tail)
		head=tail=NULL;
		else
		head=head->next;
		timer.it_value.tv_sec = 0;
 		timer.it_value.tv_usec = 0;
 		timer.it_interval.tv_sec = 0;
 		timer.it_interval.tv_usec = 0;
 		if(setitimer(ITIMER_VIRTUAL,&timer,NULL)<0) perror("Timerseterror\n");
 		incs=0;
 		if(head==NULL)
 		{
 			curr=head;
 			inmain=1;
 			timer.it_value.tv_sec = 0;
 			timer.it_value.tv_usec = 250000;
 			timer.it_interval.tv_sec = 0;
 			timer.it_interval.tv_usec = 250000;
 			setitimer(ITIMER_VIRTUAL,&timer,NULL);
 			ucontext_t G;
 			swapcontext(&G,&Main);	
		}
 		else
 		{
 			timer.it_value.tv_sec = 0;
 			timer.it_value.tv_usec = 250000;
 			timer.it_interval.tv_sec = 0;
 			timer.it_interval.tv_usec = 250000;
 			setitimer(ITIMER_VIRTUAL,&timer,NULL);
 			curr=head;
 			threadrunning=head->tid;
 			ucontext_t G;
 			swapcontext(&G,&(head->Con));
		}		
	}
	else
	{
		struct readyqueuenode * temp,* prev;
		temp=head;
		while(temp->tid!=tid)
		{
			prev=temp;
			temp=temp->next;
		}
		prev->next=temp->next;
		if(temp->next==NULL)
		{
			tail=prev;
			inmain=1;
			curr=NULL;
			ucontext_t Temp;
			timer.it_value.tv_sec = 0;
 			timer.it_value.tv_usec = 250000;
 			timer.it_interval.tv_sec = 0;
 			timer.it_interval.tv_usec = 250000;
 			setitimer(ITIMER_VIRTUAL,&timer,NULL);
			swapcontext(&Temp,&Main);
		}
		else
		{
			curr=temp->next;
			threadrunning=curr->tid;
			ucontext_t Temp;
			timer.it_value.tv_sec = 0;
 			timer.it_value.tv_usec = 250000;
 			timer.it_interval.tv_sec = 0;
 			timer.it_interval.tv_usec = 250000;
 			setitimer(ITIMER_VIRTUAL,&timer,NULL);
			swapcontext(&Temp,&curr->Con);
		}
		
	}
	qsize--;
	incs=0;
	while(1);
}
void thread_join(int tid)
{
	while(State[tid]!=NOTCREATED);
}
void thread_terminate(int tid)
{
	incs=1;
	struct readyqueuenode * temp, * prev, * nxt;
	temp=head;
	if(head->tid==tid)
		head=head->next;
	else
	{
		while(temp->tid!=tid)
		{
			prev=temp;
			temp=temp->next;
		}
		prev->next=temp->next;
	}
	State[tid]=NOTCREATED;
	incs=0;
}
void thread_create( int tid,void (*func)())
{
	ucontext_t Temp;
	getcontext(&Temp);
	Temp.uc_link=0;
	Temp.uc_stack.ss_sp=malloc(MEM);
 	Temp.uc_stack.ss_size=MEM;
 	Temp.uc_stack.ss_flags=0;
 	Array[tid]=func;
 	makecontext(&Temp,Wrapper,1, tid);
 	incs=1;
 	if(head==NULL)
 	{
 		head= (struct readyqueuenode *)malloc( sizeof(struct readyqueuenode));
 		head->tid=tid;
 		head->Con=Temp;
 		head->next=NULL;
 		tail=head;
	}
	else
	{
		struct readyqueuenode * Q=tail;
		while(Q->next!=NULL)
		{
			Q=Q->next;
		}
		Q->next= (struct readyqueuenode *)malloc( sizeof(struct readyqueuenode));
		Q->next->tid=tid;
		Q->next->Con=Temp;
		Q->next->next=NULL;
		tail=Q->next;
	}
	qsize++;
	State[tid]=READY;
	incs=0;
}
int main()
{
	head=NULL;
	tail=NULL;
	inmain=1;
	Initialize();
	printf("Here\n");
	thread_mutex_init(&mutex);
	//thread_create(0,Function1);
	thread_create(0,Function2);
	thread_create(1,Function3);
	thread_join(0);
	thread_join(1);
}
