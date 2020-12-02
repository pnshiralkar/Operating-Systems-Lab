#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void mergeSort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);


int main(){
    pid_t pid;

    int n, choice;
    printf("Enter size of array: ");
    scanf("%d", &n);

    int arr[n];


    printf("Enter elements of array: ");
    for(int i=0;i<n;i++)
        scanf("%d", &arr[i]);
    
    printf("--- Select option ---\n 1. Synchronize parent and child\n2. Orphan process\n3. Zombie process\n\n Your choice: ");
    scanf("%d", &choice);

    pid = fork();

    if(!pid){
        // In child
        printf("Child: Original array = ");
        for(int i=0;i<n;i++)
            printf("%d ", arr[i]);
        printf("\n\n");
        
        mergeSort(arr, 0, n-1);

        printf("Child: Sorted array = ");
        for(int i=0;i<n;i++)
            printf("%d", arr[i]);
        printf("\n\n");
        
        if(choice==1)
            exit(0); // Sync
        else if (choice == 2)
        {
            // Orphan
            sleep(10);
            printf("Orphan process\nChild %d adopted by %d\n", getpid(), getppid());
            system("ps -la");
            printf("\n\n");
        } 
        else
        {
            // Zombie
            printf("Child %d becomes zombie\n", getpid());
            exit(0);
        }
        
    } else if (pid==-1){
        printf("Error while forking!");
    } else {
        // In parent (pid=child's pid)
        printf("Parent: Original array = ");
        for(int i=0;i<n;i++)
            printf("%d ", arr[i]);
        printf("\n\n");
        
        mergeSort(arr, 0, n-1);

        printf("Parent: Sorted array = ");
        for(int i=0;i<n;i++)
            printf("%d ", arr[i]);
        printf("\n\n");

        if(choice==1){
            // Sync
            pid_t child_pid;
            child_pid = wait(NULL);

            printf("Parent: Child %d has finished\n", child_pid);
        } else if(choice==2){
            // Orphan
            // Do nothing so that main() returns and process exits
            printf("Parent exiting\n\n");
        } else
        {
            // Zombie
            sleep(10);
            system("ps -la");
            printf("\n\n");
        }
        
    }

    return 0;
}


void merge(int arr[], int l, int m, int r) {

  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  int L[n1], R[n2];

  for (i = 0; i < n1; i++) {
    L[i] = arr[l + i];
  }
  for (j = 0; j < n2; j++) {
    R[j] = arr[m + 1 + j];
  }

  i = 0;
  j = 0;
  k = l;

  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    }
    else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(int arr[], int l, int r) {

  if (l < r) {

    int m = l + (r - l) / 2;

    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}