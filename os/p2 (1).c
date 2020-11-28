#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
/*
 * Read from fifo1
 * count number of chars/words/lines
 * write to text file
 * write file to fifo2
 */
struct s{
	int cc,wc,lc;
};

int main(void){
	char fd1,fd2,fd3,ch;
        char buffer[255];
        mkfifo("fifo1",0666);
        mkfifo("fifo2",0666);
	fd3 = open("temp" ,O_WRONLY);
	struct s temp;
	while(1){
		fd1 = open("fifo1",O_RDONLY);
		temp.cc=0;
		temp.wc=0;
		temp.lc=0;
		while(read(fd1,&ch,1)){
			printf("%c",ch);
			temp.cc++;
			if(ch==' ') temp.wc++;
			else if(ch=='\n') temp.lc++;
		}
		printf("Char Count:\t%d\nWord Count:\t%d\nLine Count:\t%d\n",temp.cc,temp.wc,temp.lc);
		close(fd1);

		fd2 = open("fifo2",O_WRONLY);
		write(fd2,&temp,sizeof(temp));
		close(fd2);
		continue;
	}
}
