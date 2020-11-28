#include<iostream>
using namespace std;
int main()
{
      int n, temp, tt = 0, m, d = 0, i, j;
      float atat = 0, awt = 0, stat = 0, swt = 0;

      cout<<"enter no of process:"<<endl;
      cin>>n;
      int a[n], b[n], e[n], tat[n], wt[n];

      for(i=0;i<n;i++)
      {
            cout<<"enter arrival time: ";
            cin>>a[i];
      }
      for(i=0;i<n;i++)
      {
            cout<<"enter burst time: ";
            cin>>b[i];
      }

      for(i=0;i<n;i++)
      {
         for(j=i+1;j<n;j++){
            if(b[i] > b[j])
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
                temp = b[i];
                b[i] = b[j];
                b[j] = temp;
            }
        }
      }
      m = a[0];
      for(i=0;i<n;i++)
      {
            if(m > a[i])
            {
                cout<<"hi";
                m = a[i];
                d = i;
            }
      }
      tt = m;
      e[d] = tt + b[d];
      tt = e[d];

      for(i=0;i<n;i++)
      {
        if(a[i] != m)
        {
            e[i] = b[i] + tt;
            tt = e[i];
        }
      }
      for(i=0;i<n;i++)
      {
        tat[i] = e[i] - a[i];
        stat = stat + tat[i];
        wt[i] = tat[i] - b[i];
        swt = swt + wt[i];
      }
      atat = stat/n;
      awt = swt/n;
      cout<<"Process  Arrival-time(s)  Burst-time(s)  Waiting-time(s)  Turnaround-time(s)\n";

    for(i=0;i<n;i++)
    {
    cout<<"P"<<i+1<<"              "<<a[i]<<"                "<<b[i]<<"                  "<<wt[i]<<"               "<<tat[i]<<endl;
    }

    cout<<"average waiting time: "<<awt<<" average turn around time: "<<atat;
    return 0;
}
