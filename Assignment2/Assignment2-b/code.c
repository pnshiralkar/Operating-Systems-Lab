#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


void print_arr(int [],int);

int main()
{
  int n;
  printf("Main Process\nPID: %d, PPID: %d\n\nEnter the size of array: ", getpid(), getppid());
  scanf("%d", &n);

  printf("Please enter the elements in the array in ascending order: ");

  int arr[n];
  for(int i=0; i<n; i++)
	{
		scanf("%d",&arr[i]);
	}

  printf("Array is: ");
  for(int i=0; i<n; i++)
		printf(" %d, ",arr[i  ]);
  
  printf("\nForking current process.\n");

  pid_t pid = fork();

  if(pid==-1)
    printf("Error forking");
  else if(pid==0)
  {
    // Child
    char *buffer[n+1];

	buffer[0] = "./binary";
	
	for(int i=1;i<n+1;i++)
	{
		buffer[i] = malloc(20);
		snprintf(buffer[i], 20, "%d", arr[i-1]);
	}

	buffer[n] = NULL;

	execv("./binary",buffer);
  } else
  {
    // Parent
		wait(NULL);
		printf("\nParent executed successfully\n\n");
  }
  
	return 0;
}
