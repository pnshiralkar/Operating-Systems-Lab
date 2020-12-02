#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#define BUFF_SIZE 10

struct BUFF
{
    int in, out, data[BUFF_SIZE];
    sem_t full, empty;
    pthread_mutex_t b_lock;
};

struct BUFF buffer;

void init()
{
    sem_init(&buffer.empty, 0, BUFF_SIZE);
    sem_init(&buffer.full, 0, 0);
    pthread_mutex_init(&buffer.b_lock, NULL);
    srand((unsigned int)time(NULL));
    for(int i=0;i<BUFF_SIZE;i++)
        buffer.data[i]=0;
}

void printBuff()
{
    int i;
    printf("Buffer: ");
    for (i = 0; i < BUFF_SIZE; i++)
        printf("%d ", buffer.data[i]);
    printf("\n\n");
}

void *producer(void *args)
{
    int i = *(int *)args;
    while (1)
    {
        sem_wait(&buffer.empty);
        pthread_mutex_lock(&buffer.b_lock);
        //--- Critical Section Start ---
        int data = rand() % 100;
        printf("Producer:\n  produced %d\n", data);
        buffer.data[(buffer.in++ % BUFF_SIZE)] = data;
        printBuff();
        //--- Critical Section End ---
        sem_post(&buffer.full);
        pthread_mutex_unlock(&buffer.b_lock);
        sleep(rand() % 7);
    }
}

void *consumer(void *args)
{
    int i = *(int *)args;
    while (1)
    {
        sem_wait(&buffer.full);
        pthread_mutex_lock(&buffer.b_lock);
        //--- Critical Section Start ---
        printf("Consumer:\n  read %d\n", buffer.data[(buffer.out % BUFF_SIZE)]);
        buffer.data[(buffer.out++ % BUFF_SIZE)] = 0;
        printBuff();
        //--- Critical Section End ---
        sem_post(&buffer.empty);
        pthread_mutex_unlock(&buffer.b_lock);
        sleep(rand() % 7);
    }
}

int main()
{
    init();
    int np, nc;
    printf("Enter No. of Producers and Consumers\n");
    scanf("%d %d", &np, &nc);
    pthread_t tp[np], tc[nc];

    int argsp[np], argsc[nc];

    for (int i = 0; i < np; i++){
        argsp[i] = i+1;
        pthread_create(&tp[i], NULL, *producer, &argsp[i]);
    }
    for (int i = 0; i < nc; i++){
        argsc[i] = i+1;
        pthread_create(&tc[i], NULL, *consumer, &argsc[i]);
    }

    for (int i = 0; i < np; i++)
        pthread_join(tp[i], NULL);
    for (int i = 0; i < nc; i++)
        pthread_join(tc[i], NULL);
}