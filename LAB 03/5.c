/*
Write a C program using the fork (  ) system call to create a process tree with the following structure:

The parent process (P1) creates two child processes: C1 and C2.

C1 further creates two child processes: C3 and C4.

C2 creates one child process: C5.

The execution order must follow these rules strictly:

P1 starts and creates C1.

C1 starts and creates C3.

C3 finishes its execution, then C1 creates C4.

C1 finishes its execution after C4.

Once C1 finishes, P1 creates C2.

C2 creates C5 and waits for it to finish before terminating.

After all child processes have completed, P1 prints a final message indicating that all processes have finished.

Output : In every process, print the process id and parent id.*/
//CS22B1095
//REDDIPALLI SAI CHARISH 
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main() {
    pid_t child1_pid, child2_pid, child3_pid, child4_pid, child5_pid;

    printf("Parent P1:  PID is %d, My Parent PID is %d\n", getpid(), getppid());
    child1_pid = fork();
    if (child1_pid == 0) {
        printf("Child C1: PID is %d, My Parent PID is %d\n", getpid(), getppid());
        child3_pid = fork();
        if (child3_pid == 0) {
            printf("Child C3: PID is %d, My Parent PID is %d\n", getpid(), getppid());
            exit(0);
        } else if (child3_pid > 0) {
            wait(NULL);
            child4_pid = fork();
            if (child4_pid == 0) {
                printf("Child C4: PID is %d, My Parent PID is %d\n", getpid(), getppid());
                exit(0);
            } else if (child4_pid > 0) {
                wait(NULL);
                printf("Child C1: Finished after  C4\n");
                exit(0);
            }
        }
    } else if (child1_pid > 0) {
        wait(NULL);
        child2_pid = fork();
        if (child2_pid == 0) {
            printf("Child C2: My PID is %d, My Parent PID is %d\n", getpid(), getppid());
            child5_pid = fork();
            if (child5_pid == 0) {
                printf("Child C5: My PID is %d, My Parent PID is %d\n", getpid(), getppid());
            } else if (child5_pid > 0) {
                wait(NULL);
                printf("Child C2: Finished after C5\n");
                exit(0);
            }
        } else if (child2_pid > 0) {
            wait(NULL);
            printf("Parent P1: All child processes have finished. My PID is %d\n", getpid());
        }
    }

    return 0;
}
