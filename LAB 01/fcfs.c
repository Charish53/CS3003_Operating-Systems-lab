/*Write a C program to implement First Come First Serve CPU Scheduling Algorithm.

(i) Take input from the user (Process_id, Arrival Time, Burst Time), keep the information in queue. Apply FCFS algorithm on the given queue.

(ii)  Your algo should work for both scenario: (a) all tasks arrive at the same time (b) tasks may arrive at a different time.

(iii) Compute completion time, turnaround time,  waiting time and response time for each process  for the given input.

(iv) Further compute the average waiting time and average turnaround time.

(v) Print the schedule as sequence of process id for the given test input. 

Note: You use only integers for process id. Use the linked list implementation for the Queue. 



Output should print like this:

FCFS

P_id     AT    BT   FT   TAT  WT   RT


Avg waiting time:-

Avg turnaround time:-

Schedule:- 

Testcase 1

P_ID:  1,  2,  3, 4
AT    :  0,  0,  0, 0
BT   :  6,  8,  7,  3

P_id     AT    BT   FT   TAT  WT   RT

1          0     6      6       6     0     0                                           
2          0     8     14    14     6     6                                           
3          0     7     21    21    14    14                                          
4          0     3     24    24    21    21   

Avg waiting time:-10.25

Avg turnaround time:-16.25

Schedule:- 1, 2, 3, 4



Testcase 2


P_ID:  1, 2, 3, 4, 5, 6
AT:      0, 1, 2, 3, 4, 7
BT:      4, 4, 1, 2, 5, 1


P_id     AT    BT   FT   TAT  WT   RT

1          0     4     4      4       0     0                                           
2          1     4     8      7       3     3                                           
3          2     1     9      7       6     6                                           
4          3     2    11    8       6     6                                           
5          4     5    16    12     7     7                                           
6          7     1     17    10    9     9                                           
Average waiting time: 5.17                                                      
Average turn around time: 8.00  
Schedule:- 1, 2, 3, 4, 5, 6
*/
#include<stdio.h>
#include<stdlib.h>
struct P{
    int p_id;  //process_id
    int at;     //arrival time
    int bt;     //burst time
    int ft;     //finish time
    int tat;    //turn around time
    int wt;     //wait time
    int rt;     //response time
    struct P*next;
}P;
struct P*createP(int p_id,int at,int bt){
    struct P*newP=(struct P*)malloc(sizeof(struct P));
    newP->p_id=p_id;
    newP->at=at;
    newP->bt=bt;
    newP->ft=0;
    newP->tat=0;
    newP->wt=0;
    newP->rt=0;
    newP->next=NULL;
    return newP;
}
struct P* enqueueP(struct P *head,struct P *newP) {
    if (head == NULL) {
        return newP;
    } else {
        struct P*temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newP;
        return head;
    }
}
void fcfs( struct P*head) {
    int currentTime = 0;
    struct P*temp = head;
    while (temp != NULL) {
        if (currentTime < temp->at) {
            currentTime = temp->at;
        }
        temp->rt = currentTime - temp->at;
        temp->ft = currentTime + temp->bt;
        temp->tat = temp->ft- temp->at;
        temp->wt = temp->tat - temp->bt;
        currentTime = temp->ft;
        temp = temp->next;
    }
}
void printAvgTimes(struct P*head) {
    int totalTAT = 0, totalWait = 0, count = 0;
    struct P*temp = head;

    while (temp != NULL) {
        totalTAT += temp->tat;
        totalWait += temp->wt;
        count++;
        temp = temp->next;
    }

    printf("Avg waiting time: %.2f\n", (float)totalWait / count);
    printf("Avg turnaround time: %.2f\n", (float)totalTAT / count);
}
void printSchedule(struct P*head) {
    printf("P_id  AT  BT  FT  TAT  WT  RT\n");
    struct P*temp = head;

    while (temp != NULL) {
        printf("%d     %d   %d   %d   %d    %d   %d\n",
            temp->p_id, temp->at, temp->bt, temp->ft,
            temp->tat, temp->wt, temp->rt);
        temp = temp->next;
    }
    printf("Schedule: ");
    temp = head;
    while (temp != NULL) {
        printf("P%d ", temp->p_id);
        temp = temp->next;
    }
    printf("\n");
}
int main() {
    int n, p_id, at, bt;
    struct P*head = NULL;

    printf("Enter the number of processes: ");
    scanf("%d",&n);

    for (int i = 0; i < n; i++) {
        printf("Enter P_Id, Arrival Time, and Burst Time: ");
        scanf("%d %d %d", &p_id, &at, &bt);
        struct P* p = createP(p_id,at,bt);
        head=enqueueP(head,p);
    }
    fcfs(head);
    printSchedule(head);
    printAvgTimes(head);

    return 0;
}


