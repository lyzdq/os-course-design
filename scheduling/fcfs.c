#include <stdio.h>

#define MAX_PROCESS 10

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int finish_time;
    int turnaround_time;
    float weighted_turnaround_time;
} PCB;

int main() {
    PCB processes[MAX_PROCESS];
    int n, i;
    float avg_turnaround = 0, avg_weighted_turnaround = 0;

    printf("请输入进程数量: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("请输入进程 P%d 的到达时间和运行时间: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
    }

    // FCFS 调度计算
    int current_time = 0;
    printf("\n====== FCFS 调度结果 ======\n");
    printf("运行顺序: ");
    for (i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].finish_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].weighted_turnaround_time = 
            (float)processes[i].turnaround_time / processes[i].burst_time;
        current_time = processes[i].finish_time;

        printf("P%d ", processes[i].id);
        avg_turnaround += processes[i].turnaround_time;
        avg_weighted_turnaround += processes[i].weighted_turnaround_time;
    }

    printf("\n\n进程\t到达\t运行\t完成\t周转\t带权周转\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%.2f\n",
            processes[i].id,
            processes[i].arrival_time,
            processes[i].burst_time,
            processes[i].finish_time,
            processes[i].turnaround_time,
            processes[i].weighted_turnaround_time);
    }

    avg_turnaround /= n;
    avg_weighted_turnaround /= n;
    printf("\n平均周转时间: %.2f\n", avg_turnaround);
    printf("平均带权周转时间: %.2f\n", avg_weighted_turnaround);

    return 0;
}
