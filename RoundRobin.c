#include <stdio.h>
#include <stdlib.h>

struct rr {
    int pno, btime, sbtime, wtime, lst;
} p[10];

int main() {
    int pp = -1, ts, flag, count, ptm = 0, i, n, totttime = 0;

    printf("\nRound Robin Scheduling............");
    printf("\nEnter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the time slice: ");
    scanf("%d", &ts);

    printf("Enter the burst time:\n");
    for (i = 0; i < n; i++) {
        printf("\nProcess %d\t", i + 1);
        scanf("%d", &p[i].btime);
        p[i].wtime = p[i].lst = 0;
        p[i].pno = i + 1;
        p[i].sbtime = p[i].btime;
    }

    printf("\nScheduling....\n");

    do {
        flag = 0;

        for (i = 0; i < n; i++) {
            count = p[i].btime;

            if (count > 0) {
                flag = -1;
                count = (count >= ts) ? ts : count;

                printf("\nProcess %d from %d to %d", p[i].pno, ptm, ptm + count);

                ptm += count;
                p[i].btime -= count;

                if (pp != i) {
                    pp = i;
                    p[i].wtime += ptm - p[i].lst - count;
                    p[i].lst = ptm;
                }
            }
        }

    } while (flag == -1);

    // Calculate total turnaround time
    for (i = 0; i < n; i++) {
        totttime += p[i].lst;
    }

    printf("\nTotal Turnaround Time: %d\n", totttime);

    return 0;
}
