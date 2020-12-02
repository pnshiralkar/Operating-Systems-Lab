#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

int main(){
    int fd1[2], fd2[2], a=0,c=1;
    pid_t pid;
    char file[100], readBuff[100], contentBuff[1000], ch='\n';
    FILE *fp;

    pipe(fd1);
    pipe(fd2);

    printf("Enter filename: \n");
        scanf("%s", file);

    pid = fork();
    if(!pid){
        // Parent
        printf("--- Parent ---\n");
        close(fd1[0]); // Close read end of pipe1
        printf("Sending filename to child through pipe 1...\n");
        write(fd1[1], file, sizeof(file));
        close(fd1[1]);
        sleep(4);

        printf("\n--- Parent ---\n");
        close(fd2[1]);
        read(fd2[0], contentBuff, sizeof(contentBuff));
        printf("File Contents are:\n%s\n\n", contentBuff);
        close(fd2[0]);
        exit(0);

    } else {
        // Child
        printf("Child sleeping\n\n");
        sleep(2);
        printf("\n--- Child ---\n");
        close(fd1[1]); // Close write end of pipe1
        read(fd1[0], readBuff, sizeof(readBuff));
        fp = fopen(readBuff, "r");
        close(fd1[0]);
        close(fd2[0]);

        printf("Sending file contents to parent through pipe 2...\n");
        while(a!=-1){
            a = fscanf(fp, "%c", &ch);
            write(fd2[1], &ch, sizeof(ch));
        }
        close(fd2[1]);
        exit(0);
    }
}