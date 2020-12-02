#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
    int pipe, f1, f2;
    char str[100], readStr[1000];

    printf("Creating FIFO1\n");
    printf("Enter Text to send:\n");
    fgets(str, 100, stdin);
    printf("\nWriting the text into FIFO 1\n");

	f1 = open("myfifo", O_WRONLY);
    write(f1, str, strlen(str)+1);
    sleep(5);

	f2 = open("myfifo", O_RDONLY);
	read(f2, readStr, 1000);
	printf("\nThe info received from FIFO 2 : \n\n%s", readStr);
	printf("\n\n");

	unlink("myfifo");
	return 0;
}