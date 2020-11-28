#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
struct info
{
	int char_cnt;
	int word_cnt;
	int line_cnt;
};

int main(int argc,char* argv[])
{
	char fd1,fd2,fd3,ch;
        char buffer[255];
        mkfifo("fifo1",0666);
        mkfifo("fifo2",0666);
	fd3 = open("temp" ,O_WRONLY);
	struct info temp;
	while(1)
	{
		fd1 = open("fifo1",O_RDONLY);
		temp.char_cnt=0;
		temp.word_cnt=0;
		temp.line_cnt=0;
		while(read(fd1,&ch,1))
		{
			printf("%c",ch);
			temp.char_cnt++;
			if(ch==' ')
				temp.word_cnt++;
			else if(ch=='\n')
				temp.line_cnt++;
		}
		printf("Char Count: %d\nWord Count: %d\nLine Count: %d\n",temp.char_cnt,temp.word_cnt+1,temp.line_cnt);
		close(fd1);

		fd2 = open("fifo2",O_WRONLY);
		write(fd2,&temp,sizeof(temp));
		close(fd2);
		continue;
	}
}
