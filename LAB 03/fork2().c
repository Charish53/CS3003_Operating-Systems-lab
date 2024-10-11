//CS22B1095
//REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
    pid_t p;
    p=fork();
    if(p<0){
        printf("Error\n");
    }
    if(p>0){
        wait(NULL);
        int n;
        scanf("%d",&n);
        int *a=(int *)malloc(n*sizeof(int));
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        for(int i =0;i<n;i++){
            printf("\nThe element is %d and with address %p\n",a[i],&a[i]);
        }//Insertion sort 
        int key, j;
        for (int i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j -= 1;
        }
        a[j + 1] = key;
        }
        printf("The Elements in Sorted order  is :\n");
        for(int i =0;i<n;i++){
            printf("%d:%p\n",a[i],&a[i]);
        }
    }
    else{
        int n;
        scanf("%d",&n);
        int *a=(int *)malloc(n*sizeof(int));
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        for(int i =0;i<n;i++){
            printf("\nThe element is %d and with address %p\n",a[i],&a[i]);
        }//reversing array
        int i = 0;
        int j = n - 1;
        while (i < j) {
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            i++;
            j--;
        }
        printf("The elements of the reversed order is:\n");
        for(int i =0;i<n;i++){
            printf("%d:%p\n",a[i],&a[i]);
        }
    }
    return 0;
}
