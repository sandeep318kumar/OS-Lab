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
    map<int, vector<int>>mp;
    map<int, vector<int>>::iterator it;
    for(int i=0;i<n;i++)
    {
        mp[A[i]].push_back(i);
    }
    for(it = mp.begin();it != mp.end();it++)
    {
       it->second.push_back(100);
    }
    // for(auto i:mp)
    // {
    //     cout<<i.first<<"-> ";
    //     for(auto j:i.second){
    //         cout<<j<<" ";
    //     }
    //     cout<<endl;
    // }
    vector<int>v1;
    int i = 0;
    while(v1.size() < k)
    {
        if(find(v1.begin(), v1.end(), A[i]) == v1.end()){
            cout<<"Current Element is "<<A[i]<<" And "<<"The Frame Window is ";
            for(int j=0;j<v1.size();j++){
                cout<<v1[j]<<" ";
            }
            cout<<endl;
            v1.push_back(A[i]);
            cc++;
        }
        i++;
    }

    while(i<n){
        cout<<"Current Element is "<<A[i]<<" And "<<"The Frame Window is ";
        for(int j=0;j<v1.size();j++){
            cout<<v1[j]<<" ";
        }
        cout<<endl;

        if(find(v1.begin(), v1.end(), A[i]) == v1.end()){
            cc++;
            int m1 = -1, m2 = INT_MIN;

            
            for(int j=0;j<v1.size();j++){
                int m3 = 0;
                for(int l=0;l<mp[v1[j]].size();l++){
                    if(mp[v1[j]][l] > i){
                        m3 = mp[v1[j]][l];
                        break;
                    }
                }
                // cout<<v1[j]<<"->"<<m3<<", "<<m2<<endl;
                if(m3 > m2){
                    m2 = m3;
                    m1 = j;
                }
            }
            // cout<<"Max is "<<m1<<endl;
            v1[m1] = A[i];
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
