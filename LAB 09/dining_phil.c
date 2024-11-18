#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

void *fun1();
void *fun2();
pthread_mutex_t first_mutex,sec_mutex;
int main(){
    pthread_mutex_init(&first_mutex,NULL);
    pthread_mutex_init(&sec_mutex,NULL);

    pthread_t t1,t2;
    pthread_create(&t1,NULL,fun1,NULL);
    pthread_create(&t2,NULL,fun2,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("Thread joined \n");
    
}
void *fun1(){
    printf("t1 trying to acquire mutex 1 \n");
    pthread_mutex_lock(&first_mutex);
    printf("t1 acquired first mutex\n");
    sleep(1);
    printf("t1 trying to acquire second mutex\n");
    pthread_mutex_lock(&sec_mutex);
    printf("t1 acquired second mutex\n");
    //pthread_mutex_unlock(&sec_mutex);
    pthread_mutex_unlock(&first_mutex);
}

void *fun2(){
    printf("t2 trying to acquire mutex 1 \n");
    pthread_mutex_lock(&first_mutex);
    printf("t2 acquired first mutex\n");
    sleep(1);
    printf("t2 trying to acquire second mutex\n");
    pthread_mutex_lock(&sec_mutex);
    printf("t2 acquired second mutex\n");
    //pthread_mutex_unlock(&sec_mutex);
    pthread_mutex_unlock(&first_mutex);
}
