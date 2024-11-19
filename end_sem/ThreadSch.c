#include <pthread.h>
 #include <stdio.h>
 #include<stdlib.h>
 #include <unistd.h>
 #define NUM_THREADS 5
void *PrintHello(void *threadid)
{
        long id =(long) threadid;
        printf("%ld: Hello World!\n", id);
        sleep(5);
        pthread_exit(0);

}

int main( )
{
        int scope;
        pthread_t threads [NUM_THREADS];
        int rc, t;
        pthread_attr_t attr;

        /* get the default attributes */
          pthread_attr_init (&attr) ;

          printf("Set the detach state thread attribute\n");

            /* first inquire on the current scope */
            pthread_attr_setscope (&attr, PTHREAD_SCOPE_PROCESS);

            if (pthread_attr_getscope(&attr, &scope) != 0)
            fprintf(stderr, "Unable to get scheduling scope\n");
            else {
              if (scope == PTHREAD_SCOPE_PROCESS)
                printf ( "%d PTHREAD_SCOPE_PROCESS\n", scope) ;
              else if (scope == PTHREAD_SCOPE_SYSTEM)
                printf ( "%d PTHREAD_SCOPE_SYSTEM\n", scope) ;
              else
                fprintf(stderr, "Illegal scope value.\n");
              }



/* set the scheduling algorithm to PCS or SCS */

        for(t=0; t < NUM_THREADS; t++)
        {
                printf ("Creating thread %d\n", t);
                rc = pthread_create (&threads[t], &attr, PrintHello, (void *)t);
                pthread_detach(threads[t]);
                //rc = pthread_attr_destroy(&attr);

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
