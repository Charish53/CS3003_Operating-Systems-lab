/*
Write a program to create five threads using pthread library. Find the sum of first 1000 numbers using five threads, where each thread can calculate sum of 200 numbers in each (parallel counting using threads). After completion of threads operation, print the final sum from the main function.
*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int s = 0; 
pthread_mutex_t mutex; 

void *sum(void *arg) {
    int start = *((int *)arg)+1; 
    int end = start + 200;     
    
    int local_sum = 0;          
    for (int i =start; i<end; i++) {
        local_sum += i;         
    }
    pthread_mutex_lock(&mutex);
    s += local_sum;
    pthread_mutex_unlock(&mutex);
}
int main() {
    pthread_t th[5];  
    int start_indices[5];  
    pthread_mutex_init(&mutex, NULL);  
    for (int i = 0; i < 5; i++) {
        start_indices[i] = i * 200;  
        if (pthread_create(&th[i], NULL, &sum, &start_indices[i]) != 0) {
            perror("Error creating thread\n");
            return 1;
        }
        printf("Thread %d has started, calculating sum from %d to %d is \n", i, start_indices[i], start_indices[i] + 200);
    }
    for (int i = 0; i < 5; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Thread %d has executed\n", i);
    }
    pthread_mutex_destroy(&mutex);
    printf("The total sum of the first 1000 numbers is %d\n", s);
    return 0;
}
