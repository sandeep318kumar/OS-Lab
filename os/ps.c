#include<stdlib.h>
#include<unistd.h>
int main(int args,char* arg[])
{
    char * cmd="ps";
    execvp(cmd,arg);
    return 0;
}