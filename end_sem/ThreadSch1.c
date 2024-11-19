#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h> // For scheduling policies

#define NUM_THREADS 3

void* thread_function(void* arg) {
    int thread_id = *((int*)arg);
    int i=0;
    while(i<1000)
    {
      printf("id=%d\n", thread_id);
      i++;
    }

    printf("Thread %d is running.\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr[NUM_THREADS];
    struct sched_param param[NUM_THREADS];
    int thread_ids[NUM_THREADS] = {0, 1, 2};

    // Initialize attributes and create threads with round-robin scheduling
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_attr_init(&attr[i]);

        // Set scheduling policy to SCHED_RR
        int policy = SCHED_FIFO;
        pthread_attr_setschedpolicy(&attr[i], policy);


        // param[i].sched_priority = 10 + i; // Assign different priorities (10, 11, 12)
        // pthread_attr_setschedparam(&attr[i], &param[i]);

        // Create the thread
        if (pthread_create(&threads[i], &attr[i], thread_function, &thread_ids[i]) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attr[i]); // Clean up attributes
    }

    printf("All threads have finished.\n");
    return 0;
}
