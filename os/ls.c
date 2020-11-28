#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
int main(int argc, char **argv)
{
	struct dirent **namelist;
	int n;
	if(argc<1)
	{
		return 1;
	}
	else if(argc==1)
	{
		n = scandir(".", &namelist, NULL, alphasort);
	}
	else
	{
		n = scandir(argv[1], &namelist, NULL, alphasort);
	}
	if(n<0)
	{
		perror("scandir");
		exit(EXIT_FAILURE);
	}
	else
	{
		int i=0;
		while(n--)
		{
			printf("%s  ", namelist[i++]->d_name);
			free(namelist[n]);
		}
		printf("\n");
	}
	return 0;
}
