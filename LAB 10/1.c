/*Implement Banker's algorithm. Consider 6 processes and 4 types of resources.

Define the ALLOCATION and  MAX matrix  and the AVAILABLE array. Find the NEED matrix. You must ensure that your  initial allocation will always lead to the system in  safe state.

Now, take the input from the user for any process as additional request. Find that this request will lead to safe  or unsafe state.

For eg. P1=(0, 0, 1, 1)

Check whether additional P1 request will lead to safe state or not,

(a) If safe then print "SAFE", sequence of safe processes and update the  ALLOCATION, MAX and NEED Matrix

(b) If unsafe then print "UNSAFE" and discard the request without updating the above matrix.

You must take multiple times the input from the user.
*/
#include <stdio.h>
#include <stdlib.h>

#define proc 6 // Number of proc
#define res 4 // Number of res

void calculateNeed(int need[proc][res], int max[proc][res], int alloc[proc][res]) {
    for (int i = 0; i < proc; i++) {
        for (int j = 0; j < res; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}
int isSafe(int alloc[proc][res], int avail[res], int need[proc][res]) {
    int work[res], finish[proc] = {0}, safeSeq[proc];
    int count = 0;
    for (int i = 0; i < res; i++) {
        work[i] = avail[i];
    }
    while (count < proc) {
        int found = 0;
        for (int p = 0; p < proc; p++) {
            if (finish[p] == 0) {
                int j;
                for (j = 0; j < res; j++) {
                    if (need[p][j] > work[j])
                        break;
                }
                if (j == res) {
                    for (int k = 0; k < res; k++)
                        work[k] += alloc[p][k];
                    safeSeq[count++] = p;
                    finish[p] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            printf("UNSAFE\n");
            return 0;
        }
    }
    printf("SAFE Sequence: ");
    for (int i = 0; i < proc; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
    return 1;
}
void reqres(int process, int req[], int alloc[proc][res], int max[proc][res], int avail[res], int need[proc][res]) {
    int i;

    // Check if req is within the need
    for (i = 0; i < res; i++) {
        if (req[i] > need[process][i]) {
            printf("req exceeds maximum claim. req denied.\n");
            return;
        }
    }
    for (i = 0; i < res; i++) {
        if (req[i] > avail[i]) {
            printf("Not enough res available. req denied.\n");
            return;
        }
    }
    for (i = 0; i < res; i++) {
        avail[i] -= req[i];
        alloc[process][i] += req[i];
        need[process][i] -= req[i];
    }

    if (isSafe(alloc, avail, need)) {
        printf("req approved. System is in a SAFE state.\n");
    } else {
        printf("req denied. System would be UNSAFE.\n");
        // Rollback
        for (i = 0; i < res; i++) {
            avail[i] += req[i];
            alloc[process][i] -= req[i];
            need[process][i] += req[i];
        }
    }
}
int main() {
    int alloc[proc][res] = {
        {0, 1, 0, 2}, // P0
        {1, 0, 2, 1}, // P1
        {1, 3, 1, 0}, // P2
        {0, 0, 2, 0}, // P3
        {2, 2, 0, 1}, // P4
        {0, 2, 1, 2}  // P5
    };

    int max[proc][res] = {
        {3, 2, 1, 2}, // P0
        {1, 5, 3, 4}, // P1
        {2, 3, 1, 4}, // P2
        {0, 6, 3, 2}, // P3
        {4, 2, 1, 3}, // P4
        {2, 3, 3, 2}  // P5
    };

    int avail[res] = {3, 2, 2, 1}; // Available res
    int need[proc][res];
    int process;
    int req[res];

    calculateNeed(need, max, alloc);

    if (!isSafe(alloc, avail, need)) {
        printf("Initial state is UNSAFE. Adjust allocations.\n");
        return -1;
    }

    // Take multiple reqs from user
    while (1) {
        printf("\nEnter the process number (0 to %d) for additional req (or -1 to exit): ", proc - 1);
        scanf("%d", &process);
        if (process == -1) break;

        printf("Enter additional req for P%d (e.g., 1 0 2 1): ", process);
        for (int i = 0; i < res; i++) {
            scanf("%d", &req[i]);
        }

        reqres(process, req, alloc, max, avail, need);
    }

    printf("Program terminated.\n");
    return 0;
}