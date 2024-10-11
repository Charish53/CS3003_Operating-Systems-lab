/*
Write a C program which will create processes in this form of tree.

              



Note: Child 3 and Child 4 can run in any order. If you want to ensure that child3 should complete first, you can use sleep() in Child4. Use at least one time the logical operator for process creation.

Output:


Output:

From Child3: Child 3 My id is ___ and my parent id is _____.  Your Name and Roll-No, CSE/ CSE-AI Department

From Child4: Child 4 My id is ___ and my parent id is _____.  IIITDM Kancheepuram

From Child2: Child 2 My id is ___ and my parent id is _____. Kandigai

From Child1: Child 1 My id is ___ and my parent id is _____. Chennai

From Parent: Parent Process my id is ___ and my parent id is _____. India


*/

//CS22B1095
//REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    pid_t parent, child_1, child_2, child_3, child_4;
    parent = getpid();
    if ((child_2 = fork()) < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_2 == 0) {
        
        if ((child_3 = fork()) < 0) {
            perror("Fork failed");
            exit(1);
        } else if (child_3 == 0) {
            printf("From Child3: Child 3 My id is %d and my parent id is %d.Charish and CS22B1095,CSE\n", getpid(), getppid());
            exit(0);
        }
        if ((child_4 = fork()) < 0 || (child_4 == 0 && (sleep(1), printf("From Child4: Child 4 My id is %d and my parent id is %d. IIITDM Kancheepuram\n", getpid(), getppid()), exit(0), 0))) {
            if (child_4 < 0) {
                perror("Fork failed");
                exit(1);
            }
        }
        wait(NULL);
        wait(NULL);
        printf("From Child2: Child 2 My id is %d and my parent id is %d. Kandigai\n", getpid(), getppid());
        exit(0);
    }
    if ((child_1 = fork()) < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_1 == 0) {
        sleep(2); 
        printf("From Child1: Child 1 My id is %d and my parent id is %d. Chennai\n", getpid(), getppid());
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    printf("From Parent: Parent Process my id is %d and my parent id is %d. India\n", getpid(), getppid());
    return 0;
}
