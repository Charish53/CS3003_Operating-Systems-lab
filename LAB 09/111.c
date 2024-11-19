#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define n 5
#define left (phnum+4)%n
#define right (phnum+1)%n
char state[n];
int phil[] = {0, 1, 2, 3, 4};
sem_t mutex, s[n];

void test(int phnum) {
    // Check if the philosopher can eat (both left and right forks are available)
    if (state[phnum] == 'h' && state[left] != 'e' && state[right] != 'e') {
        state[phnum] = 'e'; // Mark philosopher as eating
        printf("Phil%d takes forks %d and %d\n", phnum + 1, left + 1, phnum + 1);
        printf("Phil%d is eating\n", phnum + 1);
        
        sem_post(&s[phnum]); // Signal the philosopher to proceed with eating
    }
}

void take_fork(int phnum) {
    sem_wait(&mutex); // Ensure mutual exclusion when modifying shared state
    state[phnum] = 'h'; // Philosopher is hungry
    printf("Philosopher %d is Hungry\n", phnum + 1);
    test(phnum); // Test if the philosopher can start eating
    sem_post(&mutex); // Release the mutex for other philosophers to access
    sem_wait(&s[phnum]); // Wait until the philosopher is allowed to eat
}

void put_fork(int phnum) {
    sem_wait(&mutex); // Ensure mutual exclusion when modifying shared state
    state[phnum] = 't'; // Philosopher is thinking
    printf("Philosopher %d puts down forks %d and %d\n", phnum + 1, left + 1, phnum + 1);
    printf("Philosopher %d is Thinking\n", phnum + 1);
    test(left); // Check if the left neighbor can eat
    test(right); // Check if the right neighbor can eat
    sem_post(&mutex); // Release the mutex for other philosophers to access
}

void *philospher(void *num) {
    int phnum = *(int *)num;
    while (1) {
        printf("Philosopher %d is Thinking\n", phnum + 1);
        sleep(1); // Simulate thinking
        take_fork(phnum); // Try to pick up forks and eat
        sleep(2); // Simulate eating
        put_fork(phnum); // Put down forks and start thinking
    }
}

int main() {
    pthread_t th[n];
    sem_init(&mutex, 0, 1); // Initialize mutex semaphore (binary semaphore for mutual exclusion)
    
    for (int i = 0; i < n; i++) {
        sem_init(&s[i], 0, 0); // Initialize semaphore for each philosopher to 0 (blocked initially)
        state[i] = 't'; // Initialize all philosophers as thinking
    }
    
    for (int i = 0; i < n; i++) {
        pthread_create(&th[i], NULL, philospher, &phil[i]); // Create philosopher threads
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(th[i], NULL); // Wait for all threads to finish (infinite loop prevents this from occurring)
    }
}
