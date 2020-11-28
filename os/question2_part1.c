#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
struct counts
{
	int char_cnt;
	int word_cnt;
	int line_cnt;
};

int main(int argc,char* argv[]){
    char fd1,fd2;
	char buffer[255];
	struct counts temp;
	mkfifo("fifo1",0666);
	mkfifo("fifo2",0666);
	while(1){
		fd1 = open("fifo1", O_WRONLY);
		fgets(buffer, 255, stdin);
		write(fd1, buffer, strlen(buffer));
		close(fd1);
		
		fd2 = open("fifo2",O_RDONLY);
		read(fd2, &temp, sizeof(temp));
		close(fd2);
	}
    return 0;
}

