/*
Sample code demonstrating thread scheduling and attributes has been uploaded. Please write code to explore all possible scenarios and analyze its behavior*/
//CS22B1095 REDDIPALLI SAI CHARISH
///////////===========Thread Scheduling==================//////////////////
// #include <stdio.h>
// #include<stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// #define NUM_THREADS 5
// void *PrintHello(void *threadid)
// {
// long id =(long) threadid;
// printf("%ld: Hello World!\n", id);
// sleep(5);
// pthread_exit(0);
// }

// int main( ){
// int scope;
// pthread_t threads [NUM_THREADS];
// int rc, t;
// pthread_attr_t attr;
// pthread_attr_init (&attr);
// printf("Set the detach state thread attribute\n");
// /* first inquire on the current scope */
// pthread_attr_setscope (&attr, PTHREAD_SCOPE_PROCESS);
// if (pthread_attr_getscope(&attr, &scope) != 0)
//   fprintf(stderr, "Unable to get scheduling scope\n");
// else {
//   if (scope == PTHREAD_SCOPE_PROCESS)
//     printf ( "%d PTHREAD_SCOPE_PROCESS\n", scope) ;
//   else if (scope == PTHREAD_SCOPE_SYSTEM)
//     printf ( "%d PTHREAD_SCOPE_SYSTEM\n", scope) ;
//   else
//     fprintf(stderr, "Illegal scope value.\n");
// }

// for(t=0; t < NUM_THREADS; t++){
//   printf ("Creating thread %d\n", t);
//   rc = pthread_create (&threads[t], &attr, PrintHello, (void *)t);
//   pthread_detach(threads[t]);
//   //rc = pthread_attr_destroy(&attr);

//   pthread_join (threads[t], NULL);

//   //printf("rc %d--", rc);
//   if (rc){
//   printf("ERROR; return code from pthread_create() is %d\n", rc);
//   exit(-1);
//   }
// }
// printf("End of Main Function\n");
// //  pthread_exit(NULL);

// }
/*Analysing the behaviour

Sequential Execution: Threads run one after the other due to pthread_join immediately following pthread_create.

Thread Scope Set: Sets scope to PTHREAD_SCOPE_PROCESS, allowing threads to compete for CPU within the process.

Redundant Detach & Join: Using both pthread_detach and pthread_join is unnecessary and redundant.

Blocking Main Thread: pthread_join blocks the main thread, preventing true concurrency among threads.

Attribute Management: The code could benefit from destroying thread attributes (pthread_attr_destroy) after creating all threads.
*/


//=====================Thread Scheduling 2 ============================//
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sched.h> // For scheduling policies

// #define NUM_THREADS 3

// void* thread_function(void* arg) {
//     int thread_id = *((int*)arg);
//     int i=0;
//     while(i<5)
//     {
//       printf("id=%d\n", thread_id);
//       i++;
//     }

//     printf("Thread %d is running.\n", thread_id);
//     return NULL;
// }

// int main() {
//     pthread_t threads[NUM_THREADS];
//     pthread_attr_t attr[NUM_THREADS];
//     struct sched_param param[NUM_THREADS];
//     int thread_ids[NUM_THREADS] = {0, 1, 2};

//     // Initialize attributes and create threads with round-robin scheduling
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_attr_init(&attr[i]);

//         // Set scheduling policy to SCHED_RR
//         int policy = SCHED_FIFO;
//         pthread_attr_setschedpolicy(&attr[i], policy);


//         // param[i].sched_priority = 10 + i; // Assign different priorities (10, 11, 12)
//         // pthread_attr_setschedparam(&attr[i], &param[i]);

//         // Create the thread
//         if (pthread_create(&threads[i], &attr[i], thread_function, &thread_ids[i]) != 0) {
//             perror("pthread_create");
//             return EXIT_FAILURE;
//         }
//     }

//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//         pthread_attr_destroy(&attr[i]); // Clean up attributes
//     }

//     printf("All threads have finished.\n");
//     return 0;
// }
/*

Priority-Based Execution: Threads are assigned incremental priorities (10, 11, 12), causing higher-priority threads to be scheduled before lower-priority ones, even within the SCHED_FIFO policy.

Precedence Control: Higher priority threads (with ID 2) will likely finish their tasks first, followed by lower-priority ones, depending on system scheduling.

Enhanced Execution Order: Priorities override the natural order of thread creation, so thread output may not appear sequentially by ID.

Real-Time Scheduling Impact: Due to SCHED_FIFO, thread execution follows priority strictly without time slicing, running each thread until completion before switching.

Privilege Requirements: Real-time policies and priority settings may require elevated permissions to function as expected, affecting execution on regular systems without these permissions.
*/

//========================================Thread Attributes=======================//
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define checkResults(string, val) {             \
 if (val) {                                     \
  printf("Failed with %d at %s", val, string); \
  exit(1);                                     \
 }                                              \
}

void *theThread(void *parm)
{
  printf("Entered the thread\n");
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_attr_t        attr;
  pthread_t             thread;
  int                   rc=0;

  printf("Enter Testcase - %s\n", argv[0]);

  printf("Create a default thread attributes object\n");
  rc = pthread_attr_init(&attr);
  //checkResults("pthread_attr_init() %d\n", rc);

  printf("Set the detach state thread attribute\n");
  rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
//  checkResults("pthread_attr_setdetachstate()\n", rc);

  printf("Create a thread using the new attributes\n");
  rc = pthread_create(&thread, &attr, theThread, NULL);
//  checkResults("pthread_create()\n", rc);


  printf("Destroy thread attributes object\n");
  rc = pthread_attr_destroy(&attr);
  checkResults("pthread_attr_destroy()\n", rc);
  printf("Join now \n");
  rc = pthread_join(thread, NULL);
  if (rc==0) {
     printf("Unexpected results from pthread_join()\n");
     exit(1);
  }
  //sleep(2);

  printf("Main completed\n");
  return 0;
}

/*analysing the behaviour 
Initialize and Set Attributes: Creates and sets attr to make the thread detached (PTHREAD_CREATE_DETACHED).
Create Thread: Launches theThread, which simply prints "Entered the thread."
Destroy Attributes: Cleans up attr after thread creation to free resources.
Join Attempt: Calls pthread_join on the detached thread, which is invalid for detached threads.
Error Handling: The join attempt fails, indicating pthread_join cannot be used on a detached thread, as expected.
*/
//==================================================~~~~~~~~~~~~~~=======================//





