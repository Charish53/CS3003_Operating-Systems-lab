/*
Create a program that initiates two processes. In each process, generate two threads. Use a global variable shared between the processes, and each thread will manipulate both a local variable and the global variable. The program should display:

Process ID and Thread ID for each thread
Addresses and values of both the local variable and the global variable
At the end of the execution, ensure the last statement of child process will be"child completed" and the parent process concludes with "parent completed." All threads in both the child and parent processes will execute concurrently, but the parent’s final statement will only be printed after all threads and child process  have finished.


Note: use  pthread_join(), pthread_exit() at different places and analyze the execution.
*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>
int  x=3;
void *function(void * arg){
    //pthread_exit(NULL);  /*only childcompleted and parent completed will print
    int y =6;
    x++;
    y++;
    printf("Process id : %d,Parent id: %d,Thread id : %ld\n",getpid(),getppid(),pthread_self());
    printf("Global Variable Address: %p value : %d\n",(void*)&x,x);
    printf("Local Variable Address: %p value : %d\n",(void*)&y,y);
    //pthread_exit(NULL);/*whole code prints*/
}
int main(){
    pid_t p;
    p=fork();
    if (p<0){
        perror("fork failed\n");
        return 1;
    }
    if(p ==0){
        //child process
        pthread_t th[2];
        //pthread_exit(NULL);
        for(int i =0; i<2; i++){
            if(pthread_create(&th[i],NULL,*function,NULL)!=0 ){
                perror("Error Creating Thread \n");
                return 2;
            }
        //pthread_exit(NULL);
        }
        for(int i =0; i<2;i++){
            if(pthread_join(th[i],NULL)!=0){return 3;}     
        }
       printf("Child Completed \n");
       exit(0);
       //pthread_exit(NULL);
    }
    else {
        wait(NULL);
        //pthread_exit(NULL);
        pthread_t t[2];
        for(int i =0; i<2; i++){
            if(pthread_create(&t[i],NULL,*function,NULL)!=0 ){
                perror("Error Creating Thread \n");
                return 4;
            }
            //pthread_exit(NULL);
        }
        for(int i=0;i<2;i++){
            if(pthread_join(t[i],NULL)!=0){return 5;}
        }
        printf("Parent Completed\n");
        //pthread_exit(NULL);
    }
}
