/*
Implement the reader-writer problem’s solution using semaphore.

 Use one writer thread and five reader threads. Also, clearly define the reader() function and writer() function that reader and writer thread execute separately. 

*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

sem_t reading;
sem_t writing;
pthread_mutex_t mutexBuffer;

int buffer[BUFFER_SIZE];
int read_count = 0;

void *reader(void *args) {
    do {
        sem_wait(&reading);  
        pthread_mutex_lock(&mutexBuffer);
        
        read_count++;
        if (read_count == 1) {
            sem_wait(&writing);  
        }
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&reading);  // Allow other readers
        sleep(5);
        // Reading from the buffer
        printf("Reader reading buffer: [");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d", buffer[i]);
            if (i < BUFFER_SIZE - 1) printf(", ");
        }
        printf("]\n");
        
        pthread_mutex_lock(&mutexBuffer);
        read_count--;
        if (read_count == 0) {
            sem_post(&writing);  // Last reader unlocks the writer
        }
        pthread_mutex_unlock(&mutexBuffer);
        
        sleep(1);
    } while (1);  // Infinite loop

    return NULL;
}

void *writer(void *args) {
    
    do {
        sem_wait(&writing);  // Writer wants exclusive access
        
        // Writing to the buffer
        printf("Writer writing to buffer...\n");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = rand() % 100;  // Random data for demonstration
        }
        
        printf("Writer updated buffer: [");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d", buffer[i]);
            if (i < BUFFER_SIZE - 1) printf(", ");
        }
        printf("]\n");
        
        sem_post(&writing);  // Release exclusive access for other readers/writers
        
        sleep(2);
    } while (1);  

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t w, r[5];
    
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&reading, 0, 1);    
    sem_init(&writing, 0, 1);   
    
    for (int i = 0; i < 5; i++) {
        if (pthread_create(&r[i], NULL, &reader, NULL) != 0) {
            perror("Failed to create reader thread\n");
        }
    }
    printf("Created reader threads\n");
    
    if (pthread_create(&w, NULL, &writer, NULL) != 0) {
        perror("Failed to create writer thread\n");
    }
    printf("Created writer thread\n");

    for (int i = 0; i < 5; i++) {
        if (pthread_join(r[i], NULL) != 0) {
            perror("Failed to join reader thread\n");
        }
    }
    if (pthread_join(w, NULL) != 0) {
        perror("Failed to join writer thread\n");
    }
    
    sem_destroy(&reading);
    sem_destroy(&writing);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}
