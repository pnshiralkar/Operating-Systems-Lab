#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

sem_t sem_w;
pthread_mutex_t mutex;
int nReaders = 0;
int data = 1;

void *writer(void *nw)
{
    while (1)
    {
        sem_wait(&sem_w); // Block readers
        data *= 2;
        printf("Writer %d: Write %d\n", *(int*)nw, data);
        sem_post(&sem_w); // Unblock readers

        sleep(rand()%5);
    }
}

void *reader(void *nr){
    while (1)
    {
        // Lock mutex so only one reader enters critical section
        pthread_mutex_lock(&mutex);
        nReaders++;
        if(nReaders == 1)
            sem_wait(&sem_w); // If first reader, block writers
        // Unlock mutex
        pthread_mutex_unlock(&mutex);

        printf("Reader %d: Read %d\n", *(int*)nr, data);

        // Lock mutex so only one reader enters critical section
        pthread_mutex_lock(&mutex);
        nReaders--;
        if(nReaders == 0)
            sem_post(&sem_w); // If last reader, unblock writers
        // Unlock mutex
        pthread_mutex_unlock(&mutex);
        
        sleep(rand()%5);
    }
}

void init()
{
    sem_init(&sem_w, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    srand((unsigned int)time(NULL));
}

int main(){
    init();
    int nw, nr;
    printf("Enter No. of Writers and Readers\n");
    scanf("%d %d", &nw, &nr);
    pthread_t wt[nw], rt[nr];
    
    int argsw[nw], argsr[nr];

    for (int i = 0; i < nw; i++){
        argsw[i] = i+1;
        pthread_create(&wt[i], NULL, *writer, &argsw[i]);
    }
    for (int i = 0; i < nr; i++){
        argsr[i] = i+1;
        pthread_create(&rt[i], NULL, *reader, &argsr[i]);
    }

    for (int i = 0; i < nw; i++)
        pthread_join(wt[i], NULL);
    for (int i = 0; i < nr; i++)
        pthread_join(rt[i], NULL);

    return 0;
}