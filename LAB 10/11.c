#include <stdio.h>
#include <stdlib.h>

#define proc 6
#define res 4

// Declare global variables
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

int avail[res] = {3, 2, 2, 1};
int need[proc][res];

void calculateNeed() {
    for (int i = 0; i < proc; i++) {
        for (int j = 0; j < res; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

int isSafe() {
    int work[res];
    int finish[proc] = {0};
    int safeseq[proc];
    int count = 0;

    for (int i = 0; i < res; i++) {
        work[i] = avail[i];
    }

    while (count < proc) {
        int f = 0;
        for (int p = 0; p < proc; p++) {
            if (finish[p] == 0) {
                int j;
                for (j = 0; j < res; j++) {
                    if (need[p][j] > work[j])
                        break;
                }
                if (j == res) {
                    for (int k = 0; k < res; k++) {
                        work[k] += alloc[p][k];
                    }
                    safeseq[count++] = p;
                    finish[p] = 1;
                    f = 1;
                }
            }
        }
        if (f == 0) {
            printf("Unsafe\n");
            return 0;
        }
    }

    printf("Safe sequence: ");
    for (int i = 0; i < proc; i++) {
        printf("P%d ", safeseq[i]);
    }
    printf("\n");
    return 1;
}

void reqres(int process, int req[]) {
    for (int i = 0; i < res; i++) {
        if (req[i] > need[process][i]) {
            printf("Request exceeds maximum claim. Request denied.\n");
            return;
        }
    }
    for (int i = 0; i < res; i++) {
        if (req[i] > avail[i]) {
            printf("Not enough resources available.\n");
            return;
        }
    }
    for (int i = 0; i < res; i++) {
        avail[i] -= req[i];
        alloc[process][i] += req[i];
        need[process][i] -= req[i];
    }

    if (isSafe()) {
        printf("Request approved. System is in a SAFE state.\n");
    } else {
        printf("Request denied. System would be UNSAFE.\n");
        // Rollback
        for (int i = 0; i < res; i++) {
            avail[i] += req[i];
            alloc[process][i] -= req[i];
            need[process][i] += req[i];
        }
    }
}

int main() {
    calculateNeed();

    if (!isSafe()) {
        printf("Initial state is UNSAFE. Adjust allocations.\n");
        return -1;
    }

    int process;
    int req[res];
    while (1) {
        printf("\nEnter the process number (0 to %d) for additional request (or -1 to exit): ", proc - 1);
        scanf("%d", &process);

        if (process == -1)
            break;

        printf("Enter additional request for P%d (e.g., 1 0 2 1): ", process);
        for (int i = 0; i < res; i++) {
            scanf("%d", &req[i]);
        }

        reqres(process, req);
    }

    return 0;
}
