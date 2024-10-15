/*Write a program where you create 3 threads. Consider two global matrix M1 and M2 of size 3 x 3. Perform addition, subtraction and multiplication in individual thread and store in matrix A[3][3], S[3][3] and M[3][3]. Print the individual matrix values (M1 and M2) and computed matrix of addition, subtraction, multiplication operation in the main function (parent process) .[ Once all thread process will be completed, then only main function will print.]*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
int m1[3][3];
int m2[3][3];
int add_m[3][3];
int sub_m[3][3];
int mul_m[3][3];
pthread_mutex_t mutex;

void *arthimetic(void * arg){
    int i = *(int*)arg;
    if( i == 0){
        pthread_mutex_lock(&mutex);
        for(int i =0 ;i<3;i++){
            for(int j=0;j<3;j++){
                add_m[i][j]=m1[i][j]+m2[i][j];
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    else if (i ==1){
        pthread_mutex_lock(&mutex);
        
         for(int i =0 ;i<3;i++){
            for(int j=0;j<3;j++){
                sub_m[i][j]=m1[i][j]-m2[i][j];
               
            }
        }
         pthread_mutex_unlock(&mutex);
    }else if (i ==2) {
        pthread_mutex_lock(&mutex);
        for(int i =0 ;i<3;i++){
            for(int j=0;j<3;j++){
                mul_m[i][j]=0;
                for(int k =0; k<3;k++){
                mul_m[i][j]+=m1[i][k]*m2[k][j];
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main() {
    printf("Enter matrix 1 details:\n");
    for(int i =0;i<3;i++){
        for(int j =0;j<3;j++){
            printf("\nMatrix 1(%d,%d):",i,j);
            scanf("%d",&m1[i][j]);
        }
    }
    printf("\nEnter matrix 2 details: ");
    for(int i =0;i<3;i++){
        for(int j =0;j<3;j++){
            printf("\nMatrix 2(%d,%d):",i,j);
            scanf("%d",&m2[i][j]);
        }
    }
    pthread_t th[3];  
    int start_indices[3];
    for (int i = 0; i < 3; i++) {
        start_indices[i] = i;
    }
    pthread_mutex_init(&mutex, NULL);  
    for (int i = 0; i<3; i++) {
        if (pthread_create(&th[i], NULL, &arthimetic, &start_indices[i]) != 0) {
            perror("Error creating thread\n");
            return 1;
        }
        printf("Thread %d has started\n",i);
    }
    
    printf("\nMatrix Addition Result:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", add_m[i][j]);
        }
        printf("\n");
    }
    printf("\nMatrix Subtraction Result:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", sub_m[i][j]);
        }
        printf("\n");
    }
    printf("\nMatrix Multiplication Result:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", mul_m[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i<3; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Thread %d has executed\n", i);
    }
     pthread_mutex_destroy(&mutex);
  
    return 0;
}
