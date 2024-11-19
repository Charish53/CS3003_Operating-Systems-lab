#include <pthread.h>
 #include <stdio.h>
 #define NUM_THREADS 3
void *PrintHello(void *threadid)
{
        printf("\n%d: Hello World!\n", threadid);
        pthread_exit(NULL);

}

int main( )
{
        pthread_t threads [NUM_THREADS];
        int rc, t;

        for(t=0; t < NUM_THREADS; t++)
        {
                printf ("Creating thread %d\n", t);
                rc = pthread_create (&threads[t], NULL, PrintHello, (void *) t );
                pthread_join (threads[t], NULL);
                //printf("rc %d--", rc);
                if (rc)
                {
                        printf("ERROR; return code from pthread_create() is %d\n", rc);

                        exit(-1);
                }
        }
        printf("End of Main Function\n");
        //  pthread_exit(NULL);

}
