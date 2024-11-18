#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PROCESSES 100

struct data {
    int i;       // Baboon ID
    int flow;    // Direction of crossing: 1 for one direction, -1 for the other direction
};

// Global variables
sem_t mutex;                     // Semaphore for mutual exclusion
int no_of_process = 1;           // Number of baboons
int flag = 1;
int baboons_crossing = 0;        // Number of baboons currently on the rope
int current_flow_direction = 1;  // Direction of flow (1 or -1)
int rope_capacity = 5;           // Maximum number of baboons on the rope
int flow_changing = 0;

// Thread function
void *runner(void *d) {
    struct data *p = (struct data *)d;
    int passed = 0;

    do {
        sleep(1);
        passed = 0;
        sem_wait(&mutex); // Acquire the semaphore for mutual exclusion

        // Check if the current flow direction matches the baboon's flow
        if (p->flow != current_flow_direction) {
            flow_changing = 1;
            while (baboons_crossing > 0 && current_flow_direction != p->flow);
            flow_changing = 0;
            current_flow_direction = p->flow;
            flag = 1;
        } else {
            if (flow_changing == 1) {
                while ((flow_changing == 1 && current_flow_direction == p->flow) ||
                       (flow_changing == 0 && current_flow_direction != p->flow));
            }
        }

        // Critical Section: Baboon starts crossing the rope
        printf("ENTER B: %d - \tflow %d\t - \t%d / %d\n", p->i, current_flow_direction, baboons_crossing, rope_capacity);
        baboons_crossing++; // Increment the count of baboons on the rope
        sleep(1); // Simulate crossing
        printf("EXIT B: %d - \tflow %d\t - \t%d / %d\n", p->i, current_flow_direction, baboons_crossing, rope_capacity);
        baboons_crossing--; // Decrement the count after crossing

        sem_post(&mutex); // Release the semaphore
        flag = 0;
        passed = 1;

    } while (1);

    pthread_exit(0);
}

int main() {
    // Get the number of baboons from the user
    printf("Enter the number of baboons: ");
    scanf("%d", &no_of_process);

    if (no_of_process < 1 || no_of_process > MAX_PROCESSES) {
        printf("Invalid number of baboons. Please enter a number between 1 and %d.\n", MAX_PROCESSES);
        return -1;
    }

    // Initialize the semaphore
    sem_init(&mutex, 0, rope_capacity);

    pthread_t tid[no_of_process];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    srand(time(NULL));
    struct data d[no_of_process];

    // Create threads for each baboon
    for (int i = 0; i < no_of_process; i++) {
        d[i].i = i;
        d[i].flow = (i % 2 == 0) ? 1 : -1; // Assign flow direction alternately
        pthread_create(&tid[i], &attr, runner, &d[i]);
    }

    // Wait for all threads to complete (infinite loop, so it will run indefinitely)
    for (int i = 0; i < no_of_process; i++) {
        pthread_join(tid[i], NULL);
    }

    // Clean up the semaphore
    sem_destroy(&mutex);
    return 0;
}
