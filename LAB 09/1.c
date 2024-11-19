#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

char state[N]; // 'H' for Hungry, 'T' for Thinking, 'E' for Eating
int phil[N] = {0, 1, 2, 3, 4};
sem_t mutex, S[N];

void test(int phnum) {
    if (state[phnum] == 'H' && state[LEFT] != 'E' && state[RIGHT] != 'E') {
        state[phnum] = 'E'; // Eating
        printf("Philosopher %d takes forks %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        sem_post(&S[phnum]);
    }
}

void take_fork(int phnum) {
    sem_wait(&mutex);
    state[phnum] = 'H'; // Hungry
    printf("Philosopher %d is Hungry\n", phnum + 1);
    test(phnum);
    sem_post(&mutex);
    sem_wait(&S[phnum]);
}

void put_fork(int phnum) {
    sem_wait(&mutex);
    state[phnum] = 'T'; // Thinking
    printf("Philosopher %d puts down forks %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is Thinking\n", phnum + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void* philosopher(void* num) {
    int phnum = *(int*)num;
    while (1) {
        printf("Philosopher %d is Thinking\n", phnum + 1);
        sleep(1);
        take_fork(phnum);
        sleep(2); // Eating
        put_fork(phnum);
    }
}

int main() {
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < N; i++) {
        sem_init(&S[i], 0, 0);
        state[i] = 'T'; // Initialize all philosophers to Thinking
    }

    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
