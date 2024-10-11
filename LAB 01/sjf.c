/*
 Write a C program to implement the Shortest Job First Scheduling Algorithm.

(i) Take input from the user only once (Process_id, Arrival Time, Burst Time) and apply SJF.

(ii) Your algorithm should work for both scenarios: (a) all tasks arrive at the same time (b) tasks may arrive at a different time.

(iii) Compute completion time, turnaround time,  waiting time and response time for each process  for the given input.

(iv) Further compute the average waiting time and average turnaround time.

(v) Print the schedule as sequence of process id for the given input.

Note: You can use only integers for process id. Use the min heap data structure (using linked list) to implement SJF Queue.

Testcase 1

P_ID:  1,  2,  3, 4
AT    :  0,  0,  0, 0
BT   :  6,  8,  7,  3

PID   AT    BT    CT    TAT   WT    RT                                          
--------------------------------------------------                              
4     0     3     3     3     0     0                                           
1     0     6     9     9     3     3                                           
3     0     7     16    16    9     9                                           
2     0     8     24    24    16    16                                          
Average waiting time: 7.00                                                      
Average turn around time: 13.00     
Schedule:-  4, 1, 3, 2


Testcase 2

P_ID:  1, 2, 3, 4, 5, 6
AT:      0, 1, 2, 3, 4, 7
BT:      4, 4, 1, 2, 5, 1

PID   AT    BT    CT    TAT   WT    RT                                          
--------------------------------------------------                              
1         0     4     4       4     0     0                                           
3         2     1     5       3     2     2                                           
4         3     2     7       4     2     2                                           
6         7     1     8       1     0     0                                           
2        1     4     12      11    7     7                                           
5        4     5     17      13    8     8                                           
Average waiting time: 3.17                                                      
Average turn around time: 6.00 
Schedule:-  1, 3, 4, 6, 2, 5
*/
#include <stdio.h>
#include <stdlib.h>
struct P {
    int p_id;  // process id 
    int at;    //arrival time
    int bt;    // burst time
    int ft;    // finishtime
    int tat;   // turn around time
    int wt;    // wait time
    int rt;    // response time
    struct P *next;
};
struct P* createP(int p_id, int at, int bt) {
    struct P* newP = (struct P*)malloc(sizeof(struct P));
    newP->p_id = p_id;
    newP->at = at;
    newP->bt = bt;
    newP->ft = 0;
    newP->tat = 0;
    newP->wt = 0;
    newP->rt = 0;
    newP->next = NULL;
    return newP;
}
struct P* insertProcess(struct P* head, struct P* newP) {
    if (head == NULL || newP->bt < head->bt) {
        newP->next = head;
        return newP;
    }
    struct P* current = head;
    while (current->next != NULL && current->next->bt <= newP->bt) {
        current = current->next;
    }
    newP->next = current->next;
    current->next = newP;
    return head;
}
void sjf(struct P* head) {
    int currentTime = 0;
    struct P* current = head;

    while (current != NULL) {
        if (currentTime < current->at) {
            currentTime = current->at;
        }
        current->rt = currentTime - current->at;
        current->ft = currentTime + current->bt;
        current->tat = current->ft - current->at;
        current->wt = current->tat - current->bt;
        currentTime = current->ft;

        current = current->next;
    }
}
void printAvgTimes(struct P* head) {
    int totalTAT = 0, totalWT = 0, count = 0;
    struct P* current = head;

    while (current != NULL) {
        totalTAT += current->tat;
        totalWT += current->wt;
        count++;
        current = current->next;
    }
    printf("Avg waiting time: %.2f\n", (float)totalWT / count);
    printf("Avg turnaround time: %.2f\n", (float)totalTAT / count);
}
void printSchedule(struct P* head) {
    printf("P_id  AT  BT  FT  TAT  WT  RT\n");
    struct P* current = head;

    while (current != NULL) {
        printf("%d     %d   %d   %d   %d    %d   %d\n",
            current->p_id, current->at, current->bt, current->ft,
            current->tat, current->wt, current->rt);
        current = current->next;
    }

    printf("Schedule: ");
    current = head;
    while (current != NULL) {
        printf("P%d ", current->p_id);
        current = current->next;
    }
    printf("\n");
}
int main() {
    int n, p_id, at, bt;
    struct P* head = NULL;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter Process ID, Arrival Time, and Burst Time: ");
        scanf("%d %d %d", &p_id, &at, &bt);
        struct P* newP = createP(p_id, at, bt);
        head = insertProcess(head, newP);
    }

    sjf(head);
    printSchedule(head);
    printAvgTimes(head);

    return 0;
}


