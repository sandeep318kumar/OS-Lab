#include<bits/stdc++.h>
using namespace std;

struct process{
    int burstTime;
    int priority;
    int process_number;
};
typedef struct process *prc;
bool comp(prc A, prc B)
{
    return A->priority < B->priority;
}
int main()
{
    int n;
    cout<<"Enter Total Number of Process:";
    cin>>n;
    vector<prc>vec;
    cout<<"\nEnter Burst Time and Priority\n";
    for(int i=0;i<n;i++)
    {
        int a, b;
        cout<<"\nP["<<i+1<<"]\n";
        cout<<"Burst Time: ";
        cin>>a;
        cout<<"Priority:  ";
        cin>>b;
        prc p = new process;
        p->burstTime = a;
        p->priority = b;
        p->process_number = i + 1;
        vec.push_back(p);
                //contains process number
    }
    // less number, higher priority
    sort(vec.begin(), vec.end(), comp);
    int wt[n], tat[n];
    wt[0]=0;
    int total = 0, pos, temp, avg_wt, avg_tat, i, j;        //waiting time for first process is zero

    for(i=1;i<n;i++)
    {
        wt[i]=0;
        for(j=0;j<i;j++)
            wt[i] += vec[j]->burstTime;

        total += wt[i];
    }

    avg_wt = total/n;
    total = 0;

    cout<<"\nProcess\t    Burst Time    \tWaiting Time\tTurnaround Time";
    for(i=0;i<n;i++)
    {
        tat[i] = vec[i]->burstTime + wt[i];
        total += tat[i];
        cout<<"\nP["<<vec[i]->process_number<<"]\t\t  "<<vec[i]->burstTime<<"\t\t    "<<wt[i]<<"\t\t\t"<<tat[i];
    }

    avg_tat = total/n;     //average turnaround time
    cout<<"\n\nAverage Waiting Time: "<<avg_wt;
    cout<<"\nAverage Turnaround Time: "<<avg_tat;

    return 0;
}
