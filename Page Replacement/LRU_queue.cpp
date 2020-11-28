#include<bits/stdc++.h>
using namespace std;

void display(list<int>l)
{
    cout<<"Frame window is ";
    for(auto i:l)
        cout<<i<<" ";
    cout<<endl;
}
int solve(int A[], int n, int k)
{
    int cc = 0;
    list<int>l;
    map<int, list<int>::iterator>mp;
    for(int i=0;i<n;i++)
    {
        
        if(mp.find(A[i]) == mp.end())
        {
            cout<<"Element is "<<A[i]<<" And ";
            display(l);
            cc++;
            if(l.size() == k)
            {
                int last = l.back();
                l.pop_back();
                mp.erase(last);
            }
        }
        else
        {
            l.erase(mp[A[i]]);
        }
        l.push_front(A[i]); 
        mp[A[i]] = l.begin();

    }
    return cc;
}
int main()
{
    int n;
    cout<<"Enter string length: ";
    cin>>n;
    int A[n];
    cout<<"Enter string elements: ";
    for(int i=0;i<n;i++)
    {
        cin>>A[i];
    }

    cout<<"Enter frame length: ";
    int k;
    cin>>k;

    // int A[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    // // int A[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    // int n = sizeof(A)/sizeof(A[0]);
    // int k = 3;
    cout<<"\nTotal Page Faults Are "<<solve(A, n, k);
    return 0;
}