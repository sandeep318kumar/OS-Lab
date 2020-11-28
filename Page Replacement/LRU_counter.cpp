#include<bits/stdc++.h>
using namespace std;
void display(vector<int>v)
{
    for(auto i:v)
        cout<<i<<" ";
    cout<<endl;
}
int solve(int A[], int n, int k)
{
    int cc = 0;
    vector<int>v1, v2;
    int i = 0;
    while(v1.size() < k)
    {
        

        for(int j=0;j<v2.size();j++)
            v2[j]++;
        if(find(v1.begin(), v1.end(), A[i]) == v1.end()){
            cout<<"Current Element is "<<A[i]<<" And "<<"The Frame Window is ";
            display(v1);
            v1.push_back(A[i]);
            v2.push_back(1);
            cc++;
        }
        i++;
    }
    while(i<n)
    {
        

        for(int j=0;j<v2.size();j++)
            v2[j]++;

        if(find(v1.begin(), v1.end(), A[i]) == v1.end()){
            cout<<"Current Element is "<<A[i]<<" And "<<"The Frame Window is ";
            display(v1);
            int m1 = -1, m2 = INT_MAX;
            for(int j=0;j<v2.size();j++){
                if(v2[j] < m2){
                    m2 = v2[j];
                    m1 = j;
                }
            }
            v1[m1] = A[i];
            v2[m1] = 1;
            cc++;
        }
        i++;
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