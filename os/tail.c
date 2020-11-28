#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int rmin(int a, int b)
{
  if(a > b) return a;
  else return b;
}

int cat(int argc, char *argv[]) {
	int i; // Loop index variables
	char buffer[4096];
	int fd, count; 
	i=1;
	int lcount;
	int flag=0;
	if (argv[1][0]=='-' && argv[1][1]=='n')
	{
		i=3;
		flag=1;
		sscanf(argv[2],"%d",&lcount);
	}
	else 
		lcount=1;
	for(i; i<argc; i++) {

		if (strcmp(argv[i], "-") == 0) {
			fd = fileno(stdin);
		}
		else {
		// Open the i’th argument file for reading
			fd = open(argv[i], O_RDONLY, 0);
		}
		if (fd<0) {
			sprintf(buffer, "Error: Opening file %s :", argv[i]);
			perror(buffer);
			continue;
		}
		int cline=0;
		while((count = read(fd, buffer, sizeof(buffer))) > 0) {
			for(int j=0;j<count;j++){
				if(buffer[j]=='\n') cline++;
			}
			cline++;
			int lineon=1;
			for(int j=0;j<count;j++)
			{
				if(lineon >= (cline-lcount) || flag==0) 
					printf("%c",buffer[j]);
				if(buffer[j]=='\n') 
					lineon++;
			}	
			//write(fileno(stdout), buffer, count);
		}
		// If current file is not stdin, then close the file
		if (strcmp(argv[i], "-") != 0)
			close(fd);
	} 
	return 0;
}
int main(int argc, char *argv[]) {
	int b=0;
	return cat(argc, argv);
}