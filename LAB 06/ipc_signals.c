/*
Write a C program that demonstrates handling the following signals: SIGINT, SIGFPE and SIGKILL. The program should create a parent process that subsequently creates a child process. The child process should create a grandchild process. Ensure that the grandchild process completes its execution first, followed by the child process, and finally the parent process.

In the grandchild process, handle the floating-point exception for a division by zero condition. From the user-defined signal handler, terminate the current grandchild process using SIGKILL.
In the child process, handle the interrupt signal (SIGINT) using Ctrl+C. From the user-defined signal handler, terminate the current child process.
In the parent process, to handle a user-defined signal in a parent process using (SIGUSR1), you need to set up a signal handler.
Note:
The program should demonstrate all three cases: default handling, ignoring the signal, and using a user-defined signal handler. (You can comment out different cases to see their effects.)
  */
//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
void sig_handler_fpe(){
    printf("Caught Float Point Error \n");
    kill(getpid(),SIGKILL);
    //signal(SIGINT,SIG_IGN);
	//raise(SIGINT);
    //printf("I am in still in sig_handler ()\n");
	//exit(0);
}
void  sig_handler_int(){
    printf("\nCaught the child process .\n------Ended the Child Process------\n ");
    //kill(getpid(),SIGKILL);
    exit(0);
}
void sig_handler_main(){
    printf("I am in parent  process \n");
    //kill(getpid(),SIGKILL);
    //exit(0);
}
int main(){
    pid_t pid,gc;
     pid = fork();
    if(pid == 0){
        if(pid < 0){return 1;}
        gc= fork();
        if(gc < 0){return 2;}
        if(gc == 0){
            int i = 10;
			int j =0;
			//signal(SIGFPE,sig_handler_fpe);
			//signal(SIGFPE,SIG_IGN);
			//signal(SIGFPE,SIG_DFL);
			printf("I am in grandchild process\n");
			i=i/j;
			printf("i--%d\n", i);
			printf("I am in grandchild process\n");
        }else if (gc >0){
            wait(NULL);
            signal(SIGINT,sig_handler_int);
            //signal(SIGINT,SIG_IGN);
            //signal(SIGINT,SIG_DFL);
            printf("Value of SIGINT %d\n",SIGINT);
            while(1){
                printf("Running.. Child Process ..(press Ctrl+c to stop)\n");
                sleep(1);
            }
        }
    }else {
        wait(NULL);
        printf("Child processes Done ! Now in Parent Process......\n");
        signal(SIGINT,SIG_IGN);
        signal(SIGUSR1,sig_handler_main);
        raise(SIGUSR1);
        //signal(SIGINT,SIG_IGN);
    }
    
}
