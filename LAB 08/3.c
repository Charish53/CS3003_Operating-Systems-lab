#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

sem_t reading;  // Semaphore to allow multiple readers
sem_t writing;  // Semaphore to ensure exclusive writer access
sem_t mutex;    // Semaphore to protect `read_count`
int buffer[BUFFER_SIZE];
int read_count = 0;  // Number of active readers

void *reader(void *args) {
    do {
        sem_wait(&reading);  // Allow a reader to access the buffer
        sem_wait(&mutex);    // Protect `read_count` during modification
        
        read_count++;
        if (read_count == 1) {
            sem_wait(&writing);  // First reader blocks the writer
        }
        
        sem_post(&mutex);    // Release protection on `read_count`
        sem_post(&reading);  // Allow other readers to enter
        
        // Reading the buffer
        printf("Reader reading buffer: [");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d", buffer[i]);
        }
        printf("]\n");
        
        sem_wait(&mutex);    // Protect `read_count` during modification
        read_count--;
        if (read_count == 0) {
            sem_post(&writing);  // Last reader unblocks the writer
        }
        sem_post(&mutex);    // Release protection on `read_count`
        
        sleep(1);
    } while (1);  // Infinite loop

    return NULL;
}

void *writer(void *args) {
    do {
        sem_wait(&writing);  // Writer waits for exclusive access
        
        // Writing to the buffer
        printf("Writer writing to buffer...\n");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = rand() % 5;  // Random data for demonstration
        }
        
        printf("Writer updated buffer: [");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d", buffer[i]);
        }
        printf("]\n");
        
        sem_post(&writing);  // Release exclusive access
        
        sleep(2);
    } while (1);  // Infinite loop

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t w, r[5];
    
    // Initialize semaphores
    sem_init(&reading, 0, 1);  // Allow multiple readers
    sem_init(&writing, 0, 1);  // Exclusive writer access
    sem_init(&mutex, 0, 1);    // Protect `read_count`
    
    // Create reader threads
    for (int i = 0; i < 5; i++) {
        if (pthread_create(&r[i], NULL, &reader, NULL) != 0) {
            perror("Failed to create reader thread\n");
        }
    }
    printf("Created reader threads\n");
    
    // Create writer thread
    if (pthread_create(&w, NULL, &writer, NULL) != 0) {
        perror("Failed to create writer thread\n");
    }
    printf("Created writer thread\n");

    // Join reader threads
    for (int i = 0; i < 5; i++) {
        if (pthread_join(r[i], NULL) != 0) {
            perror("Failed to join reader thread\n");
        }
    }
    // Join writer thread
    if (pthread_join(w, NULL) != 0) {
        perror("Failed to join writer thread\n");
    }
    
    // Destroy semaphores
    sem_destroy(&reading);
    sem_destroy(&writing);
    sem_destroy(&mutex);

    return 0;
}
