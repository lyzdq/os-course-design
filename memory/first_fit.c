#include <stdio.h>
#include <stdbool.h>

#define MAX_PARTITIONS 10

typedef struct {
    int id;
    int size;
    int start_addr;
    bool allocated;
    int process_id;
} Partition;

void show_partitions(Partition p[], int n) {
    printf("\n分区ID\t起始地址\t大小\t状态\t分配进程\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t%s\t",
            p[i].id, p[i].start_addr, p[i].size,
            p[i].allocated ? "已分配" : "空闲");
        if (p[i].allocated) printf("P%d\n", p[i].process_id);
        else printf("无\n");
    }
}

int main() {
    Partition partitions[MAX_PARTITIONS];
    int n, i, choice, process_size, process_id;

    printf("请输入初始空闲分区数量: ");
    scanf("%d", &n);

    int addr = 0;
    for (i = 0; i < n; i++) {
        partitions[i].id = i + 1;
        partitions[i].allocated = false;
        partitions[i].process_id = -1;
        printf("请输入分区%d的大小: ", i + 1);
        scanf("%d", &partitions[i].size);
        partitions[i].start_addr = addr;
        addr += partitions[i].size;
    }

    while (1) {
        printf("\n1.分配内存  2.回收内存  3.显示状态  4.退出\n选择: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("请输入进程ID和需要的内存大小: ");
            scanf("%d %d", &process_id, &process_size);

            bool found = false;
            for (i = 0; i < n; i++) {
                if (!partitions[i].allocated && partitions[i].size >= process_size) {
                    partitions[i].allocated = true;
                    partitions[i].process_id = process_id;
                    printf("进程 P%d 分配到分区%d\n", process_id, partitions[i].id);
                    found = true;
                    break;
                }
            }
            if (!found) printf("分配失败\n");
        }
        else if (choice == 2) {
            printf("请输入要回收的进程ID: ");
            scanf("%d", &process_id);
            bool found = false;
            for (i = 0; i < n; i++) {
                if (partitions[i].allocated && partitions[i].process_id == process_id) {
                    partitions[i].allocated = false;
                    partitions[i].process_id = -1;
                    printf("进程 P%d 占用的分区%d 已回收\n", process_id, partitions[i].id);
                    found = true;
                    break;
                }
            }
            if (!found) printf("未找到该进程\n");
        }
        else if (choice == 3) {
            show_partitions(partitions, n);
        }
        else {
            break;
        }
    }

    return 0;
}
