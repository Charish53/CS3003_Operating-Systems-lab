/*
The Dining Philosophers problem involves five philosophers seated around a table, each represented as a thread in a concurrent programming environment. Each philosopher can be in one of three states: THINKING, where they are engaged in thought; HUNGRY, where they desire to eat but must wait for utensils; and EATING, where they are consuming their meal. The goal is to implement this scenario using semaphores to ensure that at any given time, only two philosophers can eat simultaneously, thus managing access to the shared resources.

The implementation should include functions to handle the actions of thinking, becoming hungry, and eating. The think function should indicate when a philosopher is thinking with a print statement ("Philosopher X is thinking"). The hungry function should manage the transition to the HUNGRY state, attempting to acquire a semaphore to access the utensils and print a message ("Philosopher X is hungry"). Finally, the eat function should indicate when a philosopher is eating with a print statement ("Philosopher X is eating"). Additionally, the program must ensure that state transitions are managed properly to prevent deadlock and ensure fair access to resources


*/

//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
sem_t mutex;
sem_t S[N];
void test(int phnum)
{
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
        // state that eating
        state[phnum] = EATING;
        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n",phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        // sem_post(&S[phnum]) has no effect
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}
// take up chopsticks
void take_fork(int phnum)
{
    sem_wait(&mutex);
    // state that hungry
    state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);
    // eat if neighbours are not eating
    test(phnum);
    sem_post(&mutex);
    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);
    sleep(1);
}
// put down chopsticks
void put_fork(int phnum)
{
    sem_wait(&mutex);
    // state that thinking
    state[phnum] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n",phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT);
    test(RIGHT);

    sem_post(&mutex);
}

void* philosopher(void* num)
{
    while (1) {
        int* i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}

int main()
{
    int i;
    pthread_t thread_id[N];
    // initialize the semaphores
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL,philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }
    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}
 

