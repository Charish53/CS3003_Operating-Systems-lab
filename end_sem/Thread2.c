#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int gD=5;
void *thread_fn1(void *arg){
        long id =(long) arg;
        printf ("Thread ID is ---> %lu\n", pthread_self());
        printf("Starting thread--> %lu\n",id);
        printf("Hi\n");
        gD=gD-1;
        printf("Exiting thread--> %lu\n", id);
        return NULL;
}
void *thread_fn2(void *arg){
        long id =(long) arg;
        int ret;
        printf ("Thread ID is ---> %lu\n", pthread_self());
        printf("Starting thread --->%lu\n", id);
        printf("Helloo %d\n", gD);
        //sleep(5);
        printf("Exiting thread -->%lu\n", id);


      pthread_exit(0);

}

int main(){

        pid_t pid = fork();
        if(pid == 0) {
                printf("Child => PPID: %d PID: %d\n", getppid(), getpid());
                //  exit(EXIT_SUCCESS);
        }
        else if(pid > 0) {

                printf("Waiting for child process to finish.\n");
                wait(NULL);
                printf("Parent => PID: %d\n", getpid());
                printf("Child process finished.\n");
        }
        else {
                printf("Unable to create child process.\n");
        }

        pthread_t t1,t2;

        pthread_create(&t1,NULL,thread_fn1,(void *)1);
        pthread_join(t1,NULL);
        pthread_create(&t2,NULL,thread_fn2,(void *)2);
        pthread_join(t2, NULL);



        return 0;
}
