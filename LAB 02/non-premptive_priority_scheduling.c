/*
Implement a non-preemptive priority scheduling approach.
 (i) Maintain one global variable which represents the processor current time (Current_Proc_time).  Take input for a few new processes from the user at time when the current  process finishes its execution. New processes arrival time will always be the same or lesser than Current_Proc_time. Provide the input as P_id, arrival time (may arrives at  different time), priority of each process, and burst time. 
(ii) Show the Completion time, turnaround time, waiting time, and response time for each process. Assume there is no context switch overhead.
(iii) Compute average turnaround time, waiting time, and response time.
(iv)  Print the schedule for the given input.

Note: 

You can use only integers for process id. Use structures for keeping information of each process.
Use the array implementation(dynamically using malloc) for the Queue. At starting your ready queue can hold at most 3 processes. Resize double your queue if more processes arrive.
A Lower number represents the highest priority and the highest number represents the lowest priority. Deallocate the ready queue after completion of last process.

Test Case1
P_ID:  1,  2, 3, 4, 5, 6
AT:      0, 1, 2, 6, 8, 10
PR:     2,  3, 1, 2, 4,  3
BT:      4, 4,  6, 2, 5, 1

Non-Preemptive Priority Scheduling: PID, AT, PR, BT, FT, TAT, WT, RT



Schedule for Non- Preemptive priority scheduling: 1, 3, 4, 2, 6, 5
*/
//CS22B1095
//REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <stdlib.h>
typedef struct {
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int priority;   // Priority (lower number means higher priority)
    int burst;      // Burst Time
    int completion; // Completion Time
    int tat;        // Turnaround Time
    int wt;         // Waiting Time
    int rt;         // Response Time
    int remaining;  // Remaining Burst Time
} Process;

int Current_Proc_time = 0;

void fcfs(Process *proc, int n, int *gantt_chart, int *gantt_size) {
    int completed = 0;

    while (completed < n) {
        int idx = -1;
        int highest_priority = 1e9;

        for (int j = 0; j < n; j++) {
            if (proc[j].arrival <= Current_Proc_time && proc[j].completion == 0) {
                if (proc[j].priority < highest_priority) {
                    highest_priority = proc[j].priority;
                    idx = j;
                }
            }
        }

        if (idx != -1) {
            if (proc[idx].remaining == proc[idx].burst) {
                proc[idx].rt = Current_Proc_time - proc[idx].arrival;
            }

            Current_Proc_time += proc[idx].remaining;
            proc[idx].completion = Current_Proc_time;
            proc[idx].tat = proc[idx].completion - proc[idx].arrival;
            proc[idx].wt = proc[idx].tat - proc[idx].burst;
            gantt_chart[(*gantt_size)++] = proc[idx].pid; 
            completed++;
        } else {
            Current_Proc_time++;
        }
    }
}

void printGanttChartTable(Process *proc, int *gantt_chart, int gantt_size) {
    printf("\nGantt Chart Table:\n");
    printf("PID\tAT\tPR\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < gantt_size; i++) {
        int pid = gantt_chart[i];
        for (int j = 0; j < gantt_size; j++) {
            if (proc[j].pid == pid) {
                printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                       proc[j].pid, proc[j].arrival, proc[j].priority,
                       proc[j].burst, proc[j].completion, proc[j].tat,
                       proc[j].wt, proc[j].rt);
                break;
            }
        }
    }
}

void calculateAverages(Process *proc, int n) {
    double total_tat = 0, total_wt = 0, total_rt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += proc[i].tat;
        total_wt += proc[i].wt;
        total_rt += proc[i].rt;
    }
    printf("\nAverage Turnaround Time: %.2lf", total_tat / n);
    printf("\nAverage Waiting Time: %.2lf", total_wt / n);
    printf("\nAverage Response Time: %.2lf\n", total_rt / n);
}

void printGanttChart(int *gantt_chart, int gantt_size) {
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < gantt_size; i++) {
        printf("|  P%d  ", gantt_chart[i]);
    }
    printf("|\n");
}

int main() {
    int n = 3, max_size = 3;
    int gantt_chart[100], gantt_size = 0; 

    Process *proc = (Process *)malloc(max_size * sizeof(Process));

    int count = 0;
    char more_input;

    do {
        if (count >= max_size) {
            max_size *= 2;
            proc = (Process *)realloc(proc, max_size * sizeof(Process));
        }

        printf("Enter process details (PID, Arrival Time, Priority, Burst Time):\n");
        scanf("%d %d %d %d", &proc[count].pid, &proc[count].arrival, &proc[count].priority, &proc[count].burst);
        proc[count].completion = 0;
        proc[count].remaining = proc[count].burst;

        count++;

        printf("Do you want to enter more processes? (y/n): ");
        scanf(" %c", &more_input);
    } while (more_input == 'y');

    fcfs(proc, count, gantt_chart, &gantt_size);
    printGanttChartTable(proc, gantt_chart, gantt_size);
    printGanttChart(gantt_chart, gantt_size);
    calculateAverages(proc, count);

    free(proc);
    return 0;
}
