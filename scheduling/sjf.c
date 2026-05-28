#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESS 10

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int finish_time;
    int turnaround_time;
    float weighted_turnaround_time;
    bool completed;
} PCB;

int main() {
    PCB processes[MAX_PROCESS];
    int n, i, j;
    float avg_turnaround = 0, avg_weighted_turnaround = 0;

    printf("请输入进程数量: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        processes[i].id = i + 1;
        processes[i].completed = false;
        printf("请输入进程 P%d 的到达时间和运行时间: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
    }

    // SJF 非抢占调度
    int current_time = 0, completed_count = 0;
    printf("\n====== SJF 调度结果 ======\n");
    printf("运行顺序: ");

    while (completed_count < n) {
        int shortest = -1;
        int min_burst = 999999;

        for (i = 0; i < n; i++) {
            if (!processes[i].completed && 
                processes[i].arrival_time <= current_time &&
                processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                shortest = i;
            }
        }

        if (shortest == -1) {
            current_time++;
            continue;
        }

        processes[shortest].finish_time = current_time + processes[shortest].burst_time;
        processes[shortest].turnaround_time = 
            processes[shortest].finish_time - processes[shortest].arrival_time;
        processes[shortest].weighted_turnaround_time = 
            (float)processes[shortest].turnaround_time / processes[shortest].burst_time;
        processes[shortest].completed = true;
        current_time = processes[shortest].finish_time;
        completed_count++;

        printf("P%d ", processes[shortest].id);
        avg_turnaround += processes[shortest].turnaround_time;
        avg_weighted_turnaround += processes[shortest].weighted_turnaround_time;
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
