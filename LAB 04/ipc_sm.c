/*
Write a program that demonstrates inter-process communication (IPC) using shared memory. The program should:

Create two processes: a parent process and a child process.
Either the parent or child process can create the shared memory region.
The child process should write at least 10 integer values on the shared memory.
The parent process should read from the shared memory, modify it and display the data. Check the sample input and accordingly modify the shared data. 
Sample Input
Child Process writing : 1, 5, 11, 18, 24, 35, 47, 53, 62, 78

Parent Process reading:

1, 5, 11, 18, 24, 35, 47, 53, 62, 78

Parent Process modifying:---

Parent Process displaying:

3, 7, 13,  20, 26, 37, 49, 55, 64, 80



Important: Examine various scenarios by commenting out some system calls and provide your analysis as comments directly at the bottom of the code.

*/
// //CS22B1095
// //REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHMSIZE 100 
int main() {
    int shmid;
    key_t key;
    int *shm, *s;
    int a[15];
    key = 4095; // Shared memory key
    
    pid_t p;
    p = fork(); // Create child process
    
    int n;
    if (p < 0) {
        printf("Error forking\n");
        exit(0);
    }
    else if (p == 0) {
        printf("Enter the number of integers: ");
        scanf("%d", &n);
        if ((shmid = shmget(key, SHMSIZE, IPC_CREAT | 0666)) < 0) {
            perror("shmget");
            exit(1);
        }
        printf("Shared memory ID in child: %d\n", shmid);
        if ((shm = (int*)shmat(shmid, NULL, 0)) == (int*)-1) {
            perror("shmat");
            exit(1);
        }
        printf("Shm address in child is %p\n", shm);
        shm[0] = n;
        printf("Child Process Writing:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        s = shm + 1;  
        for (int i = 0; i < n; i++) {
            *s++ = a[i];
            printf("%d ", a[i]);
        }
        printf("\n");
        int k=shmdt(shm);
        printf("Shared memory id %d\n",k);
        // int v =shmctl(shmid,IPC_RMID,NULL);
        // printf("Shared memonry id %d\n",v);
        // printf("\n");
        // exit(0);
    }
    else {
        wait(NULL);
        if ((shmid = shmget(key, SHMSIZE, IPC_CREAT | 0666)) < 0) {
            perror("shmget");
            exit(1);
        }
        printf("Shared memory ID in parent: %d\n", shmid);
        if ((shm = (int*)shmat(shmid, NULL, 0)) == (int*)-1) {
            perror("shmat");
            exit(1);
        }
        printf("Shm address in parent is %p\n", shm);
        printf("Shm memory in parent is %p\n",shm);
        n = shm[0];
        s = shm + 1; 
        printf("Parent Process Reading:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", *s++);
        }
        printf("\n");
        s = shm + 1;
        printf("Parent Process Modifying:\n");
        for (int i = 0; i < n; i++) {
            *s++ += 2;
        }
        s = shm + 1;
        printf("Parent Process Displaying:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", *s++);
        }
        printf("\n");
        int k=shmdt(shm);
        printf("Shared memory id %d\n",k);
        int v =shmctl(shmid,IPC_RMID,NULL);
        printf("Shared memonry id %d\n",v);
        printf("\n");
    }
    return 0;
}
/*using sleep since the shared memory is being created in parent process , so child has to wait until the its created
on running the code multiple times , shmctl is giving error as that instance is not getting cleared from the memory 
using shmctl in child process makes the parent read garbage values 
if key is different , the code throws the error no such file or directory
rows an error of invalid argument and child process throws an error
when shmat is commneted out , the code gives segmentation fault
when shmdt is commneted out , the code does not throw any error memory leaks might occur as both process is attached to the shm
when shmctl is commented out , the code may not throw an error , but may lead to bottlenecks later as the shared memory is not erased*/
