#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t mutex, wrt;
int readcount = 0,x=2;

void *writer(void *arg) {
    while (1) {
        sleep(1);
        sem_wait(&wrt);
        printf("Writer is writing.\n");
        sleep(1);
        printf("Writer has finished writing.\n");
        sem_post(&wrt);
        sleep(1);
    }
}

void *reader(void *arg) {
    while (1) {
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1)
        sem_wait(&wrt);
        sem_post(&mutex);

        printf("Reader is reading.\n");
        sleep(1);
        printf("Reader has finished reading.\n");

        sem_wait(&mutex);
        readcount--;
        if (readcount == 0)
        sem_post(&wrt);
        sem_post(&mutex);

        sleep(1);
    }
}

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_t w1, r1, r2, r3, r4, r5;

    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&r3, NULL, reader, NULL);
    pthread_create(&r4, NULL, reader, NULL);
    pthread_create(&r5, NULL, reader, NULL);

    pthread_join(w1, NULL);
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(r4, NULL);
    pthread_join(r5, NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
