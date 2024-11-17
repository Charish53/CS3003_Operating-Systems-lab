/*
Given 2 threads, schedule the order of execution in such a way that you must guarantee that Statement A1 happens before Statement B2 and Statement B1 happens before Statement A2.

Thread A

//Statement A1

//Statement A2



Thread B

//Statement B1

//Statement B2



Note: Your solution should not enforce too many constraints. For ex: there is no mention about the order of execution of A1 and B1. Therefore, either order should be possible.

Utilize functions from the semaphore.h library for synchronization.*/

//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/wait.h>
sem_t mutex_a;
sem_t mutex_b;
void *thread_a(void * arg){
    
    printf("This is statement A1\n");
    sem_post(&mutex_b);
    sem_wait(&mutex_a);
    printf("This is statement A2\n");
    
}
void *thread_b(void*arg){
    printf("This is statement B1\n");
    sem_post(&mutex_a);
    sem_wait(&mutex_b);
    printf("This is statement B2\n");
    
}
int main(){
   sem_init(&mutex_a,0,0);
   sem_init(&mutex_b,0,0);
   pthread_t t1,t2;
   pthread_create(&t1,NULL,thread_a,NULL);
   pthread_create(&t2,NULL,thread_b,NULL);
   pthread_join(t1,NULL);
   pthread_join(t2,NULL);
   sem_destroy(&mutex_a);
   sem_destroy(&mutex_b);
   return 0;
    
}