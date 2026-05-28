#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESS 10

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int finish_time;
    int turnaround_time;
    float weighted_turnaround_time;
    bool completed;
} PCB;

int main() {
    PCB processes[MAX_PROCESS];
    int n, i;
    float avg_turnaround = 0, avg_weighted_turnaround = 0;

    printf("请输入进程数量: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        processes[i].id = i + 1;
        processes[i].completed = false;
        printf("请输入进程 P%d 的到达时间、运行时间和优先级(数字越小优先级越高): ", i + 1);
        scanf("%d %d %d", &processes[i].arrival_time, 
              &processes[i].burst_time, &processes[i].priority);
    }

    int current_time = 0, completed_count = 0;
    printf("\n====== 优先级调度结果 ======\n");
    printf("运行顺序: ");

    while (completed_count < n) {
        int highest = -1;
        int min_priority = 999999;

        for (i = 0; i < n; i++) {
            if (!processes[i].completed && 
                processes[i].arrival_time <= current_time &&
                processes[i].priority < min_priority) {
                min_priority = processes[i].priority;
                highest = i;
            }
        }

        if (highest == -1) {
            current_time++;
            continue;
        }

        processes[highest].finish_time = current_time + processes[highest].burst_time;
        processes[highest].turnaround_time = 
            processes[highest].finish_time - processes[highest].arrival_time;
        processes[highest].weighted_turnaround_time = 
            (float)processes[highest].turnaround_time / processes[highest].burst_time;
        processes[highest].completed = true;
        current_time = processes[highest].finish_time;
        completed_count++;

        printf("P%d ", processes[highest].id);
        avg_turnaround += processes[highest].turnaround_time;
        avg_weighted_turnaround += processes[highest].weighted_turnaround_time;
    }

    printf("\n\n进程\t到达\t运行\t优先级\t完成\t周转\t带权周转\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%.2f\n",
            processes[i].id,
            processes[i].arrival_time,
            processes[i].burst_time,
            processes[i].priority,
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
