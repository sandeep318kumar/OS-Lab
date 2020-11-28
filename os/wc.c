#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
int cat(int argc, char *argv[]) {
	int i; // Loop index variables
	char buffer[4096];
	int fd, count; 
	i=1;
	int flag=0;
	fd = open(argv[1], O_RDONLY, 0);
	if (fd<0) {
		sprintf(buffer, "Error: Opening file %s :", argv[1]);
		perror(buffer);
	}
	int cline=0;
	int charc=0;
	int cword=0;
	while((count = read(fd, buffer, sizeof(buffer))) > 0) {
		for(int j=0;j<count;j++)
		{
			if(buffer[j]=='\n') cline++;
			if(buffer[j]==' ') cword++;
			charc++;
		}
		//write(fileno(stdout), buffer, count);
		printf("%d %d %d\n", cline, cword, charc);
	}
	// If current file is not stdin, then close the file
	if (strcmp(argv[i], "-") != 0){
		close(fd);
	} 
	return 0;
}
int main(int argc, char *argv[]) {
	int b=0;
	return cat(argc, argv);
}