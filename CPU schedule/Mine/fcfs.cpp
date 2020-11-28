#include<iostream>
using namespace std;

int main()
{
    int n, bt[20], wt[20], tat[20], averageWaitingTime = 0, aveargeTurnAroundTime = 0;
    cout<<"Enter total number of processes(maximum 20):";
    cin>>n;

    cout<<"\nEnter Process Burst Time\n";
    for(int i=0;i<n;i++)
    {
        cout<<"P["<<i+1<<"]:";
        cin>>bt[i];
    }

    wt[0] = 0;
    for(int i= 1;i<n;i++)
    {
        wt[i] = 0;
        for(int j=0 ;j<i;j++)
            wt[i] += bt[j];
    }

    cout<<"\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time";


    //calculating turnaround time
    for(int i=0;i<n;i++)
    {
        tat[i] = bt[i] + wt[i];
        averageWaitingTime += wt[i];
        aveargeTurnAroundTime += tat[i];
        cout<<"\nP["<<i+1<<"]"<<"\t\t"<<bt[i]<<"\t\t"<<wt[i]<<"\t\t"<<tat[i];
    }

    averageWaitingTime /= n;
    aveargeTurnAroundTime /= n;
    cout<<"\n\nAverage Waiting Time: "<<averageWaitingTime;
    cout<<"\nAverage Turnaround Time: "<<aveargeTurnAroundTime;
    return 0;
}