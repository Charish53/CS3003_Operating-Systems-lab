#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Global variables
int children = 0, adult = 0, children_waiting = 0, adult_exiting = 0;
sem_t adultsem;
sem_t childrensem;
pthread_mutex_t mutex;

// Helper function to get the minimum of two numbers
int minimum(int a, int b) {
    return (a < b) ? a : b;
}

// Child thread function
void *childThread() {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (children < 3 * adult) {
            children++;
            printf("Child enters the room\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
            pthread_mutex_unlock(&mutex);
        } else {
            children_waiting++;
            printf("A child tried to enter but has to wait\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
            pthread_mutex_unlock(&mutex);
            sem_wait(&childrensem);
        }

        sleep(2); // Simulate time spent in the room

        pthread_mutex_lock(&mutex);
        children--;
        printf("Child exits the room\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
               children, adult, children_waiting, adult_exiting);

        // If an adult is waiting to exit and the ratio is maintained, allow the adult to leave
        if (adult_exiting && (children <= 3 * (adult - 1))) {
            adult_exiting--;
            adult--;
            printf("An adult exits the room\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
            sem_post(&adultsem);
        }

        // Allow a waiting child to enter if the 1:3 ratio is maintained
        if (children_waiting && (children < 3 * adult)) {
            sem_post(&childrensem);
            children_waiting--;
            children++;
            printf("A waiting child is allowed to enter\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

// Adult thread function
void *adultThread() {
    while (1) {
        pthread_mutex_lock(&mutex);
        adult++;
        printf("An adult enters the room\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
               children, adult, children_waiting, adult_exiting);

        // Allow up to 3 waiting children to enter for each new adult
        while (children_waiting > 0 && children < 3 * adult) {
            sem_post(&childrensem);
            children_waiting--;
            children++;
            printf("A waiting child enters the room\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
        }
        pthread_mutex_unlock(&mutex);

        sleep(3); // Simulate time spent in the room

        pthread_mutex_lock(&mutex);
        if ((children <= 3 * (adult - 1)) || ((adult > 1) && (children_waiting == 0 && children == 0))) {
            adult--;
            printf("An adult exits the room\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
        } else {
            adult_exiting++;
            printf("An adult wants to exit but has to wait\nChildren = %d; Adults = %d; Waiting children = %d; Exiting adults = %d\n",
                   children, adult, children_waiting, adult_exiting);
            pthread_mutex_unlock(&mutex);
            sem_wait(&adultsem);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

// Function to terminate the program after a certain time
void *terminateProgram(void *arg) {
    sleep(3); // Run for 20 seconds
    printf("Terminating program...\n");
    exit(0);
}

int main() {
    sem_init(&adultsem, 0, 0);
    sem_init(&childrensem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    int nchildren, nadults;
    
    // Get user input for the number of children and adults
    printf("Enter the number of child threads to create: ");
    scanf("%d", &nchildren);
    printf("Enter the number of adult threads to create: ");
    scanf("%d", &nadults);

    pthread_t *children_threads = (pthread_t *)malloc(sizeof(pthread_t) * nchildren);
    pthread_t *adult_threads = (pthread_t *)malloc(sizeof(pthread_t) * nadults);

    pthread_t terminator;
    pthread_create(&terminator, NULL, terminateProgram, NULL);

    // Create child threads
    for (int i = 0; i < nchildren; i++) {
        pthread_create(&children_threads[i], NULL, childThread, NULL);
    }

    // Create adult threads
    for (int i = 0; i < nadults; i++) {
        pthread_create(&adult_threads[i], NULL, adultThread, NULL);
    }

    // Join threads (though they won't actually complete due to the infinite loop)
    for (int i = 0; i < nadults; i++) {
        pthread_join(adult_threads[i], NULL);
    }
    for (int i = 0; i < nchildren; i++) {
        pthread_join(children_threads[i], NULL);
    }

    free(children_threads);
    free(adult_threads);
    sem_destroy(&adultsem);
    sem_destroy(&childrensem);
    pthread_mutex_destroy(&mutex);
    return 0;
}
