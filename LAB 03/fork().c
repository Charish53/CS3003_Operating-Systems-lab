/*Write a C program that will create one child process.

Define one global array and take input from the user.

Perform the following operations in a different process.

(i) Parent Process:-


(a) Update the array by subtracting 3 from each element of the array.

(b)Print the elements, address of the elements and find minimum element in the array

(c) Print id and parent id of the process

(ii) Child Process:-

(a) Update the array by adding 2 on each element of the array.

(b) Print the elements, address of the elements and find maximum element in the array

(c)print id and parent id
*/
//CS22B1095
//REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
int a[100];
int main(){
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    pid_t p;
    p=fork();
    if(p<0){
        printf("---Error---\n");
    }
    if(p>0){
        for(int i =0;i<n;i++){
            a[i]-=3;
        }
        printf("\n---The Elements of the array are :---\n");
        int min=a[0];
        for(int i =0;i<n;i++){
            printf("\nThe element is %d and with address %p\n",a[i],&a[i]);
            if(a[i]<min){
                min=a[i];
            }
        }
         printf("\nThe minimum element of the array is :%d\n",min);
        printf("\n I am a Parent Process %d and my parent is %d \n",getpid(),getppid());
    }
    else{
        for(int i =0;i<n;i++){
            a[i]+=2;
        }
         printf("\n---The Elements of the array are :---\n");
        int max=a[0];
        for(int i =0;i<n;i++){
            printf("\nThe element is %d and with address %p\n",a[i],&a[i]);
            if(a[i]>max){
                max=a[i];
            }
        }
        printf("\nThe Maximum element of the array is : %d\n",max);
        printf("\n I am a Child Process %d and my parent is %d \n",getpid(),getppid());
    }
}
