/*
C program using pipe() and fork() where the parent process reads a string from the user, sends it to the child process through a pipe, and the child process reads the string from the pipe and displays it.

Important: Examine various scenarios and provide your analysis as comments directly at the bottom of the code.

1.  Commenting Out System Calls, specially close (  ) at certain places.

2. What will happen, If both ends of the pipe are closed before any read or write operations are performed.

3. What happens if you call read() and write() on the same pipe in the same process.

4. What happens if you write multiple times to the pipe without corresponding reads.

5. Investigate and analyze any other scenarios that you come across
*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<unistd.h>
int main()
{
    int pipefd[2];
    int returnstatus;
    int pid;
    char writemsg[20];
    char readmsg[20];
    // char write1msg[20];
    // char read1msg[20];
    returnstatus=pipe(pipefd);
    if(returnstatus==-1)
    {
        printf("unable to create pipe\n");
        return 1;
    }
    pid=fork();
    if(pid<0)
    printf("error in fork");
    else if(pid>0)
    {
        close(pipefd[0]);
        scanf("%s",writemsg);
        //close(pipefd[1]);
        write(pipefd[1],writemsg,sizeof(writemsg));
        printf("parent process-writing from pipe %s \n",writemsg);
        //close(pipefd[1]);
        scanf("%s",writemsg);
        write(pipefd[1],writemsg,sizeof(writemsg));
        printf("parent process-writing from pipe %s \n",writemsg);
        //close(pipefd[1]);
    }
    else
    {
        close(pipefd[1]);
        //close(pipefd[0]);
        read(pipefd[0],readmsg,sizeof(readmsg));
        printf("child process-reading from pipe %s \n",readmsg);
        //close(pipefd[0]);
        read(pipefd[0],readmsg,sizeof(readmsg));
        printf("child process-reading from pipe %s \n",readmsg);
    }
    return 0;
}
/*
1. Commenting Out System Calls, especially close() at certain places:
   - If we comment out close(fd[0]) in the parent, the parent process will not close the read end, but since it's not reading from the pipe, this has no immediate effect.
   - If we comment out close(fd[1]) in the child, the write end will remain open, and the read operation in the child will block indefinitely, as the pipe will expect more data from the parent (or any other process with the write end open).
   - If we comment out the close(fd[1]) in the parent, the pipe’s write end will stay open, and the child process will wait indefinitely on the read() call, as it will think there might be more data coming.

2. What happens if both ends of the pipe are closed before any read or write operations are performed:
   - If both ends of the pipe (i.e., both fd[0] and fd[1]) are closed before any read or write operations, no data can be transferred. Any attempt to write() or read() will result in an error. Specifically, write() will return a -1 with the error EPIPE (Broken pipe), and read() will return 0, indicating an end-of-file (EOF) condition.

3. What happens if you call read() and write() on the same pipe in the same process:
   - If the same process calls both read() and write() on the same pipe, it will likely result in a deadlock. The process would write to the pipe and then attempt to read from it, but since no other process is writing to the pipe, the read() call will block indefinitely, waiting for data that will never come. This causes the process to hang.

4. What happens if you write multiple times to the pipe without corresponding reads:
   - If multiple writes are performed to the pipe without corresponding reads, the pipe can become full (pipe has limited buffer size, typically 4096 bytes on many systems). Once the pipe is full, the write() operation will block, waiting for the pipe to be read from and free up space. If no reads happen, the writing process will be blocked indefinitely until space becomes available in the pipe buffer.

5. Investigating Other Scenarios:
   - Reading from an empty pipe*: If a process tries to read from an empty pipe and no other process has the write end open, the read() call will return 0, which indicates the end of the pipe (EOF). This signifies that no more data will be written to the pipe.
   - Closing one end of the pipe early*: If the parent closes its write end early, before the child has finished reading, the child might not get all the data, or it could encounter EOF if no more data is coming through.
   - Writing to a closed pipe*: If the child or parent tries to write to the pipe after the other process has closed the reading end (close(fd[0])), the writing process will receive a SIGPIPE signal and terminate with an error. This usually manifests as a broken pipe error.
*/
