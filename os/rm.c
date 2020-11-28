#include<stdio.h>
int main(int argc, char * argv[])
{
    int isdeleted;
    isdeleted=remove(argv[1]);
    if(!isdeleted) return 0;
    else
    {
        printf("Delete not successful :(\n");
        return 1;
    }
}