#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdio.h>

int main()
{
	int n,fd, sz;
	char buff[50], c[100];	

	printf("Enter text to write:\n");
	n = read(0, buff, 50);

	// creating a new file using open.
	fd = open("file.txt",O_CREAT | O_RDWR, 0666);

	// O_RDONLY: read only, 
    // O_WRONLY: write only, 
    // O_RDWR: read and write, 
    // O_CREAT: create file if it doesn’t exist, 
    // O_EXCL: prevent creation if it already exists

	printf("\nWriting to the file and closing ...\n");
	write(fd, buff, n);
	close(fd);

  	fd = open("file.txt", O_RDONLY); 
	sz = read(fd, c, 100); 
	printf("\nCalling read function ...\n");
	printf("%d bytes read.\n", sz); 
	c[sz] = '\0'; 
	printf("\nFile Contents: \n%s\n", c); 

	// closing the file
	close(fd);
	
	return 0;	
}