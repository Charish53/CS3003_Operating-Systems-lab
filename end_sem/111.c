#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 1

// Shared buffer and message
char buffer[BUFFER_SIZE];
int buffer_filled = 0; // Flag for buffer state

// Semaphores for synchronization
sem_t empty, full;
pthread_mutex_t mutex;

// Writer function
void *writer(void *arg) {
    char *message = "Message from Writer";
    while (1) {
        sem_wait(&empty); // Wait for empty buffer
        pthread_mutex_lock(&mutex);

        // Write to buffer
        snprintf(buffer, BUFFER_SIZE, "%c", message[rand() % strlen(message)]);
        printf("Writer: Written '%s' to buffer\n", buffer);
        buffer_filled = 1;

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Signal buffer is full
        sleep(1); // Simulate delay
    }
}

// Producer function
void *producer(void *arg) {
    while (1) {
        sem_wait(&full); // Wait for buffer to be full
        pthread_mutex_lock(&mutex);

        if (buffer_filled) {
            printf("Producer: Produced item '%s'\n", buffer);
            buffer_filled = 0;
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Signal buffer is empty
        sleep(1); // Simulate delay
    }
}

// Consumer function
void *consumer(void *arg) {
    while (1) {
        sem_wait(&full); // Wait for buffer to be full
        pthread_mutex_lock(&mutex);

        if (!buffer_filled) {
            printf("Consumer: Consumed item '%s'\n", buffer);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Signal buffer is empty
        sleep(1); // Simulate delay
    }
}

// Reader function
void *reader(void *arg) {
    while (1) {
        sem_wait(&empty); // Wait for empty buffer
        pthread_mutex_lock(&mutex);

        printf("Reader: Read '%s' from buffer\n", buffer);

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Signal buffer is full
        sleep(1); // Simulate delay
    }
}

int main() {
    pid_t pid;
    pthread_t writer_thread, producer_thread, consumer_thread, reader_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, 1); // Buffer starts as empty
    sem_init(&full, 0, 0);  // Buffer is not full
    pthread_mutex_init(&mutex, NULL);

    // Fork the process
    pid = fork();

    if (pid == 0) { // Child process handles reader-writer problem
        printf("Child Process: Handling Reader-Writer Problem\n");
        pthread_create(&writer_thread, NULL, writer, NULL);
        pthread_create(&reader_thread, NULL, reader, NULL);

        pthread_join(writer_thread, NULL);
        pthread_join(reader_thread, NULL);
    } else if (pid > 0) { // Parent process handles producer-consumer problem
        printf("Parent Process: Handling Producer-Consumer Problem\n");
        pthread_create(&producer_thread, NULL, producer, NULL);
        pthread_create(&consumer_thread, NULL, consumer, NULL);

        pthread_join(producer_thread, NULL);
        pthread_join(consumer_thread, NULL);
    } else {
        perror("Fork failed");
        exit(1);
    }

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
