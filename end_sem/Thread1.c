#include <stdio.h>
#include <pthread.h>
#include<bits/pthreadtypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
void *threadfunc1(void *p)
{
        pthread_exit(0);

        printf ("Thread parent ID is ---> %d\n", getpid());
        printf ("Thread ID is outside ---> %lu\n", pthread_self());

        for(int i=0; i<3; i++)
        {
                printf ("Inside Thread ID is ---> %lu and i=%d\n", pthread_self(), i);

        }


  pthread_exit(0);

}


int main ( )
{

        pthread_t Thread1, Thread2;
        int t, u;

        printf ("Parent ID is ---> %d\n", getpid( ));
        pthread_create (&Thread1, NULL, threadfunc1, NULL);
        //pthread_join (Thread1, NULL);

        u=pthread_create (&Thread2, NULL, threadfunc1, NULL);
        pthread_join (Thread1, NULL);
        t=pthread_join (Thread2, NULL);
        //pthread_join (Thread1, NULL);
        printf("Return status %d and %d\n", u, t);



        // sleep(1);
        printf ("No more Threads!\n");

        //



}
