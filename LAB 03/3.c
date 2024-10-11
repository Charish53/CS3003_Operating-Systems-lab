/*
Write two programs (Given Program31.c, Program32.c and Program33.c).

In Program31.c,  write a code that create two child process. 

In Parent Process:

After both child processes have finished executing, the parent process will resume execution and print the following message::

the Institution name in the parent process,
the id of the process,
 the parent id, and 
the process IDs of both child processes that have completed their execution.
To explore all variants of the wait (  )[wait (  ), waitpid (  ), ..] family in Linux, follow steps below.
Each variant should be commented out except for one, which should be uncommented for testing.
In Child 1 process:

Print the ID of the process parent id and return value of the fork (  ) for the child process.
 The process executes a new program written in Program32.c. The executable file for Program32 .c is (program32).  In Program32.c, write or compute anything of your choice.
In Child 2 process:

Print the ID of the process parent id and return value of the fork (  ) for the child process.
The process executes a new program written in Program33.c. The executable file for Program33.c is (program33).    In Program33.c, write or compute anything of your choice.
To explore all variants of the exec (  ) family in Linux  [ execl(), execlp(), execle(), execvp(), execv(), execve() ] and  execute new program (program33) follow the steps below.
Each method should be commented out except for one, which should be uncommented for testing.
  */
//program 3 .c
//CS22b1095
//REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    pid_t child_1,child_2,wpid;
    int s=0;
    child_1=fork();
    if(child_1<0){
        perror("Fork Failed");
        exit(1);
    }
    if(child_1==0){
        printf("Child_1:Pid %d,Parent pid: %d,Fork return value :%d\n",getpid(),getppid(),child_1);
        execlp("./program32","", NULL);
        //execl("./Program32", "Program32", NULL);
        //execle("./Program32", "program32", NULL, NULL);
        //execv("./Program32", NULL);
        //execvp("./Program32", NULL);
    }
    else if(child_1>0){
        child_2=fork();
        if(child_2<0){
        perror("Fork Failed");
        exit(1);
        }
        if(child_2==0){
            printf("Child_2:Pid %d,Parent pid: %d,Fork return value :%d\n",getpid(),getppid(),child_2);
            execlp("./program33","", NULL);
            //execl("./Program33", "Program32", NULL);
            //execle("./Program33", "program32", NULL, NULL);
            //execv("./Program33", NULL);
            //execvp("./Program33", NULL);
        }
        else if(child_2>0){
            waitpid(child_2, &s, 0);
            while ((wpid = wait(&s)) > 0);
            //printf("\nParent process resuming execution after children finish\n");
            printf("\nInstitution Name : IIITDM Kancheepuram\n");
            printf("Parent pid: %d\n", getpid());
            printf("Child_1 pid: %d\n", child_1);
            printf("Child_2 pid: %d\n", child_2);
        }
    }
    
}
//--------------- program 32.c -----------------------------//
/*

//CS22B1095
//REDDIPALLI SAI CHARISH
#include <stdio.h>
void main(){
     int n[]={1,2,3,4,5};
     int sum=0;
    for(int i=0;i<5;i++){
        sum+=n[i];
    }
     printf("%d\n",sum);
     //printf("I am vivek\n");
}
*/




//----------program 33.c----------//
//CS22B1095
//REDDIPALLI SAI CHARISH
// #include<stdio.h>
// int main(){
//     printf("I am charish\n");
//     return 0;
// }
