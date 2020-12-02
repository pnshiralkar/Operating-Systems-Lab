#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define FILLED 0 
#define READY 1 
#define NOTREADY -1 
  
struct memory { 
    char message[100]; 
    int status, pid1, pid2; 
}; 
  
struct memory* shmptr; 
  
void handleMsg(int signum) 
{ 
    if (signum == SIGUSR1) { 
        printf("Server: "); 
        puts(shmptr->message); 
    } 
} 
  
int main() 
{ 
    int pid = getpid(); 
  
    int shmid; 

    int key = 12345; 

    shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
  
    shmptr->pid1 = pid; 
    shmptr->status = NOTREADY; 
  
    signal(SIGUSR1, handleMsg); 
  
    while (1) { 
        while (shmptr->status != READY) 
            continue; 
        sleep(1); 
  
        printf("Client: "); 
        fgets(shmptr->message, 100, stdin); 
  
        shmptr->status = FILLED; 
  
        kill(shmptr->pid2, SIGUSR2); 
    } 
  
    shmdt((void*)shmptr); 
    shmctl(shmid, IPC_RMID, NULL); 
    return 0; 
} 