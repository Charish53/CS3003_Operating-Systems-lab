/*
Implement the producer – consumer problem’s solution using semaphore.

Use one producer thread and one consumer thread. Also, clearly define the produce_item() function and consume_item() function.



Utilize functions from the semaphore.h library for synchronization.

*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define BUFFER_SIZE 5  // Set buffer size to 5
#define THREAD_NUM 2

sem_t semEmpty;
sem_t semFull;
pthread_mutex_t mutexBuffer;

int buffer[BUFFER_SIZE];
int count = 0;

void print_buffer() {
    printf("Current buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < count) {
            printf("%d", buffer[i]);
        } else {
            printf("_");  // Empty slots
        }
        if (i < BUFFER_SIZE - 1) printf(", ");
    }
    printf("]\n");
}

void* produce_item(void* args) {
    while (1) {
        sleep(1);
        int x = rand() % 100;
        sleep(1);
        
        printf("Produced %d\n", x);
        
        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        
        if (count < BUFFER_SIZE) {  // Check to prevent buffer overflow
            buffer[count] = x;
            printf("Added %d at index %d\n", x, count);
            count++;
        } else {
            printf("Buffer overflow, cannot add %d\n", x);
        }
        
        print_buffer();  // Print entire buffer content
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}
void* consume_item(void* args) {
    while (1) {
        int y;
        sleep(1);
        
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        
        if (count > 0) {  // Check to prevent buffer underflow
            y = buffer[count - 1];
            printf("Removed %d from index %d\n", y, count - 1);
            buffer[count - 1] = -1;  // Optional: Reset value to indicate emptiness
            count--;
        } else {
            printf("Buffer underflow, nothing to remove\n");
        }
        
        print_buffer();  
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // Consume
        printf("Consumed %d\n", y);
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, BUFFER_SIZE);  // Initialize to BUFFER_SIZE
    sem_init(&semFull, 0, 0);
    
    for (int i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &produce_item, NULL) != 0) {
                perror("Failed to create thread");
            }
            printf("Created Producer Thread\n");
        } else {
            if (pthread_create(&th[i], NULL, &consume_item, NULL) != 0) {
                perror("Failed to create thread");
            }
            printf("Created Consumer Thread\n");
        }
    }
    
    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    
    return 0;
}
