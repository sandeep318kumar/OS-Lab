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
#define MAX 2000
const int MEM=64000;
typedef void (*f_ptr)( );
int incomingpriority;
int runningpriority;
int qsize;
int front,rear;
ucontext_t Main;
int inmain;
int incs=0;
int Status[MAXTHREADS]={NOTCREATED};
f_ptr Array[MAXTHREADS]={NULL};
struct readyqueuenode
{
	int tid;
	int priority;
	ucontext_t Con;
};
struct readyqueuenode pri_que[MAX];
void create()
{
    front = rear = -1;
}
void check(struct readyqueuenode  data)
{
    int i,j;
 
    for (i = 0; i <= rear; i++)
    {
        if (data.priority >= pri_que[i].priority)
        {
            for (j = rear + 1; j > i; j--)
            {
                pri_que[j] = pri_que[j - 1];
            }
            pri_que[i] = data;
            return;
        }
    }
    pri_que[i] = data;
}
 
void insert_by_priority(struct readyqueuenode data)
{
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        pri_que[rear] = data;
        return;
    }    
    else
        check(data);
    rear++;
}
 
/* Function to check priority and place element */
/* Function to delete an element from queue */
void delete_by_priority(int tid)
{
    int i;
 
    if ((front==-1) && (rear==-1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }
 
    for (i = 0; i <= rear; i++)
    {
        if (tid == pri_que[i].tid)
        {
            for (; i < rear; i++)
            {
                pri_que[i] = pri_que[i + 1];
            }
 
        //pri_que[i] = -99;
        rear--;
 
        if (rear == -1) 
            front = -1;
        return;
        }
    }
   // printf("\n%d not found in queue to delete", data);
}
 
/* Function to display queue elements */
void display_pqueue()
{
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }
 
    for (; front <= rear; front++)
    {
        printf(" %d ", pri_que[front].tid);
    }
 
    front = 0;
}
void Wrapper(int tid)
{
	Array[tid]();
	incs=1;
	delete_by_priority(tid);
	qsize--;
	Status[tid]=NOTCREATED;
	incs=0;
	while(1);
}
void Preempt(int signum)
{
	//display_pqueue();
	if(qsize==0||incs==1)
		return;
	if(inmain)
	{
		inmain=0;
		Status[pri_que[front].tid]=RUNNING;
		//printf("Going to preempt %d \n", pri_que[front].tid);
		swapcontext(&Main,&pri_que[front].Con);
	}
	else
	{
		inmain=1;
		if(Status[pri_que[front].tid]==RUNNING||Status[pri_que[front].tid]==WAITING)
		{
			//printf("Saved context %d \n");
			Status[pri_que[front].tid]=WAITING;
			swapcontext(&pri_que[front].Con,&Main);
		}
		else if(Status[pri_que[front].tid]==READY)
		{
			Status[pri_que[front].tid]=WAITING;
			ucontext_t Temp;
			swapcontext(&Temp,&Main);
		}
	}
}
void thread_create(int tid, void (*func)(), int priority)
{
	struct readyqueuenode data;
	data.priority=priority;
	ucontext_t Temp;
	getcontext(&Temp);
	Temp.uc_link=0;
	Temp.uc_stack.ss_sp=malloc(MEM);
 	Temp.uc_stack.ss_size=MEM;
 	Temp.uc_stack.ss_flags=0;
 	Array[tid]=func;
 	makecontext(&Temp,Wrapper,1, tid);
 	data.Con=Temp;
 	data.tid=tid;
	if(qsize==0)
	{
		incs=1;
		qsize++;
		Status[tid]=READY;
		runningpriority=data.priority;
		insert_by_priority(data);
		incs=0;
	}
	else
	{
		incs=1;
		qsize++;
		Status[tid]=READY;
		insert_by_priority(data);
		incs=0;
	}
}
void Initialize()
{
	create();
	inmain=1;
	signal(SIGVTALRM,Preempt);
	struct 	itimerval timer;
	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 250000;
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 250000;
 	setitimer(ITIMER_VIRTUAL,&timer,NULL);
}
void Function1()
{
	int i=0,j=0;
	for(i=0;i<5;i++)
	{
		printf("Counting  %d in function 1\n",i);
		//sleep(2);
		for(j=0;j<0x2fffffff;j++);
	}
	//done=1;
}
void Function2()
{
	int i=0,j=0;
	for(i=0;i<5;i++)
	{
		printf("Counting %d in function 2\n",i);
		for(j=0;j<0x2fffffff;j++);
		//sleep(2);
	}
	//done=1;
}
int main()
{
	Initialize();
	thread_create(1,Function1,8);
	thread_create(2,Function2,999999);
	while(1);
}
