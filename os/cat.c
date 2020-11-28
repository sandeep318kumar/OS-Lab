#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int cat(int argc, char *argv[]) {
    int i, j; // Loop index variables
    char buffer[4096];
    int fd, count; // Open file descriptor and

    // count of bytes to read/write
    for(i=1; i<argc; i++) {
        fd = open(argv[i], O_RDONLY, 0);
        if (fd<0) {
            sprintf(buffer, "Error: Opening file %s :", argv[i]);
            perror(buffer);
            continue;
        }
        // Read the contents from the file and display on stdout
        while((count = read(fd, buffer, sizeof(buffer))) > 0) {
            write(fileno(stdout), buffer, count);
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
