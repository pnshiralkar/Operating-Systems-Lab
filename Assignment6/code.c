#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#define N 100

sem_t sema[N];
int n;

void init()
{
    for (int i = 0; i < n; i++)
        sem_init(&sema[i], 0, 1);
}

void take_fork(int pnum)
{
    int LEFT = pnum, RIGHT = (pnum + 1) % n;
    if (pnum)
    {
        sem_wait(&sema[LEFT]);
        printf("Philisopher %d picked the fork %d Up\n", (pnum), LEFT);
        sem_wait(&sema[RIGHT]);
        printf("Philisopher %d picked the fork %d Up\n", (pnum), RIGHT);
    }
    else
    {
        sem_wait(&sema[RIGHT]);
        printf("Philisopher %d picked the fork %d Up\n", (pnum+1), RIGHT);
        sem_wait(&sema[LEFT]);
        printf("Philisopher %d picked the fork %d Up\n", (pnum+1), LEFT);
    }
}

void put_fork(int pnum)
{
    int LEFT = pnum, RIGHT = (pnum + 1) % n;

    sem_post(&sema[RIGHT]);
    printf("Philisopher %d put the fork %d Dowm\n", (pnum+1), RIGHT);
    sem_post(&sema[LEFT]);
    printf("Philisopher %d put the fork %d Down\n", (pnum+1), LEFT);
}

void *philFunc(void *pn)
{
    int pnum = *(int *)pn;
    while (1)
    {
        printf("Philosopher %d is HUNGRY\n", pnum+1);
        take_fork(pnum);
        printf("Philosopher %d is EATING\n", pnum+1);
        sleep(1);
        put_fork(pnum);
        sleep(3);
    }
}

int main()
{
    printf("Enter No. of Philosophers:\n");
    scanf("%d", &n);

    init();

    pthread_t threads[n];
    int pnums[n];

    for (int i = 0; i < n; i++)
    {
        pnums[i] = i;
        printf("Philosopher %d is THINKING\n", i+1);
        pthread_create(&threads[i], NULL, *philFunc, &pnums[i]);
    }

    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);
    
    return 0;
}