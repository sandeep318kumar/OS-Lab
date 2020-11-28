#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
int main(int args, char* arg[])
{
    int pcid;
    pcid  = atoi(arg[1]);
    kill(pcid, SIGKILL);
    return 0;
}