/*
Write a C program to implement round-robin scheduling.

(i) Take  time quantum from the user.

(ii) Maintain one variable which represents the processor time (Current_Proc_time).  Take input for a few new processes from the user at time when the current  process finishes its execution or the time quantum finishes. New processes arrival time will always be the same or lesser than Current_Proc_time. Provide the input as P_id, arrival time, and burst time. 

(Iii) Show the turnaround time, waiting time, and response time for each task.

(iv) Compute average turnaround time, waiting time, and response time.

(v) Find the count the number of context switches (including the count at the beginning of the first execution process and the end of the last execution process) and the utilization of the CPU. Assume context switch overhead is 1 time quantum.

(v) Your algorithm should work for both the scenario: (a) all tasks arrive at the same time (b) tasks may arrive at a different time

Note: You can use only integers for process id. Use the linked list implementation for the Ready Queue. 

 

Test Case1:

Input

Time quantum=3

 

PID AT BT FT TAT WT RT

1     0  5  17   17  12   1

2     1  6  29   28  22   4

3     2  3  12   10   7    7

4     3  1  14   11  10  10

5     5  5  32   27  22  13

6     6  4  34   28  24  16

Number of context switching=11

Utilization=67.6%
*/
//CS22B1095
//REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>

int tq, np, cs_count = 0;
int frt[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int ct = 0;
float twt = 0, ttat = 0;

struct P {
    int id;
    int at;
    int bt;
    int rt;
    struct P *nxt;
};

struct Q {
    struct P *f;
    struct P *r;
};

void initQ(struct Q* q) {
    q->f = q->r = NULL;
}

void enQ(struct Q* q, int id, int at, int bt) {
    struct P* np = (struct P*)malloc(sizeof(struct P));
    np->id = id;
    np->at = at;
    np->bt = bt;
    np->rt = bt;
    np->nxt = NULL;

    if (q->r == NULL) {
        q->f = q->r = np;
        return;
    }
    q->r->nxt = np;
    q->r = np;
}

struct P deQ(struct Q* q) {
    if (q->f == NULL) {
        printf("Queue is empty\n");
        return (struct P){-1, -1, -1, -1, NULL};
    }
    struct P* tmp = q->f;
    struct P dp = *tmp;
    q->f = q->f->nxt;
    if (q->f == NULL)
        q->r = NULL;
    free(tmp);
    return dp;
}

int isQEmpty(struct Q* q) {
    return q->f == NULL;
}

int main() {
    struct Q jq;
    initQ(&jq);

    printf("Enter the number of processes: ");
    scanf("%d", &np);
    
    int ids[np], ats[np], bts[np];
    
    printf("Enter the time quantum: ");
    scanf("%d", &tq);
    
    for (int i = 0; i < np; i++) {
        printf("Enter Process ID, Arrival time, and Burst time for process %d: ", i + 1);
        scanf("%d %d %d", &ids[i], &ats[i], &bts[i]);
    }
    
    int cpi = 0, tat = 0, wt = 0;
    
    printf("\nPID   AT   BT   FT   TAT  WT   RT\n"); 
    
    while (cpi < np || !isQEmpty(&jq)) {
        if (!isQEmpty(&jq)) {
            ct++;
            struct P cp = deQ(&jq);
            
            if (frt[cp.id - 1] == -1)
                frt[cp.id - 1] = ct - cp.at;
            
            if (cp.rt <= tq) {
                cs_count++;
                ct += cp.rt;
                cp.rt = 0;
                
                while (cpi < np && ats[cpi] <= ct) {
                    enQ(&jq, ids[cpi], ats[cpi], bts[cpi]);
                    cpi++;
                }
                
                tat = ct - cp.at;
                wt = tat - bts[cp.id - 1];
                ttat += tat;
                twt += wt;
                
                printf("%d     %d    %d    %d    %d    %d    %d\n", 
                       cp.id, cp.at, 
                       bts[cp.id - 1], ct, 
                       tat, wt, frt[cp.id - 1]);
                
            } else {
                cs_count++;
                ct += tq;
                cp.rt -= tq;
                
                while (cpi < np && ats[cpi] <= ct) {
                    enQ(&jq, ids[cpi], ats[cpi], bts[cpi]);
                    cpi++;
                }
                enQ(&jq, cp.id, cp.at, cp.rt);
            }
            
        } else {
            if (ats[cpi] > ct) {
                cs_count++;
                ct = ats[cpi];
            }
            enQ(&jq, ids[cpi], ats[cpi], bts[cpi]);
            cpi++;
        }
    }
    
    float cpu_util = (float)(ct - (cs_count + 1)) / ct;
    printf("\nNumber of context switches: %d\n", cs_count + 1);
    printf("\nCPU utilization: %.2f%%\n", cpu_util * 100);
    printf("\nAverage waiting time: %.2f\n", twt / np);
    printf("\nAverage turnaround time: %.2f\n", ttat / np);
    
    return 0;
}
