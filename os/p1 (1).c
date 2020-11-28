#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
/*
 * Accept sentences
 * Write to fifo1
 * read drom fifo2
 * stdout
 */
struct s{
	int cc,wc,lc;
};

int main(void){
        char fd1,fd2;
	char buffer[255];
	struct s temp;
	mkfifo("fifo1",0666);
	mkfifo("fifo2",0666);
	while(1){
		fd1 = open("fifo1",O_WRONLY);
		fgets(buffer,255,stdin);
		write(fd1,buffer,strlen(buffer));
		close(fd1);
		
		fd2 = open("fifo2",O_RDONLY);
		read(fd2,&temp,sizeof(temp));
		printf("Char Count:\t%d\nWord Count:\t%d\nLine Count:\t%d\n",temp.cc,temp.wc,temp.lc);
		close(fd2);
	}
        return 0;
}

