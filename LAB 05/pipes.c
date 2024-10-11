/*
A parent process creates a child process and the child process creates a grandchild process. The parent creates a pipe that is shared between the parent and the child. 
The parent writes a message to the pipe and the child reads it from the pipe. The child creates another pipe which is shared between the child and the grandchild. 
Note that this pipe is not available to the parent and is not the same as the one shared by the parent and the child. 
The grandchild writes another message to the pipe and the child reads it from the pipe. 
After obtaining the two messages from the pipes, the child prints the difference in lengths of the two messages.
Moreover, each of the three processes prints the PID of its respective parent process. The contents of the two messages must be taken as user inputs.

*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

#define BUFFER_SIZE 100
int main() {
    int pipe1[2], pipe2[2]; 
    pid_t pid1, pid2;
    char pipe1write[BUFFER_SIZE], pipe2write[BUFFER_SIZE];
    char pipe1read[BUFFER_SIZE], pipe2read[BUFFER_SIZE];
    if (pipe(pipe1) == -1) {
        perror("Unable to create pipe 1");
        return -1;
    }
    pid1 = fork();

    if (pid1 < 0) {
        perror("Failed to fork child process");
        return -1;
    }

    if (pid1 == 0) { 
        if (pipe(pipe2) == -1) {
            perror("Unable to create pipe 2");
            return -1;
        }
        pid2 = fork();

        if (pid2 < 0) {
            perror("Failed to fork grandchild process");
            return -1;
        }

        if (pid2 == 0) { 
            sleep(10);
            printf("\nEnter message from grandchild to child:\n");
            fgets(pipe2write, BUFFER_SIZE, stdin);
            pipe2write[strcspn(pipe2write, "\n")] = 0;
            write(pipe2[1], pipe2write, strlen(pipe2write) + 1);

            close(pipe2[0]); 
            close(pipe2[1]); 
            
            printf("Grandchild (PID: %d), Parent PID: %d\n", getpid(), getppid());
            exit(0);
        } else { // Child process

            // Close writing end of pipe1 (parent->child) and reading end of pipe2 (child->grandchild)
            close(pipe1[1]);
            close(pipe2[1]);

            // Read message from parent via pipe1
            read(pipe1[0], pipe1read, BUFFER_SIZE);

            // Read message from grandchild via pipe2
            read(pipe2[0], pipe2read, BUFFER_SIZE);


            int len1 = strlen(pipe1read);
            int len2 = strlen(pipe2read);
            printf("Message from Parent: %s\n", pipe1read);
            printf("Message from Grandchild: %s\n", pipe2read);
            printf("Difference in lengths: %d\n", len1 - len2);

         
            close(pipe1[0]);
            close(pipe2[0]);
            
            printf("Child (PID: %d), Parent PID: %d\n", getpid(), getppid());

            wait(NULL);
        }
    } else { // Parent process
        printf("Parent (PID: %d), Parent PID: %d\n", getpid(), getppid());

        printf("Enter message from parent to child:\n");
        fgets(pipe1write, BUFFER_SIZE, stdin);
        pipe1write[strcspn(pipe1write, "\n")] = 0; 

        write(pipe1[1], pipe1write, strlen(pipe1write) + 1);

        close(pipe1[0]);
        close(pipe1[1]);

        wait(NULL);
    }

    return 0;
}
