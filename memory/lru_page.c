#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_PAGES 30

int main() {
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int pages[MAX_PAGES];
    int n, m, i, j, k, faults = 0;
    bool found;

    printf("请输入物理块数量: ");
    scanf("%d", &n);
    printf("请输入页面引用串长度: ");
    scanf("%d", &m);
    printf("请输入页面引用串(空格分隔): ");
    for (i = 0; i < m; i++) {
        scanf("%d", &pages[i]);
    }

    for (i = 0; i < n; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    printf("\n====== LRU 页面置换过程 ======\n");

    for (i = 0; i < m; i++) {
        found = false;

        for (j = 0; j < n; j++) {
            if (frames[j] == pages[i]) {
                found = true;
                last_used[j] = i;
                break;
            }
        }

        if (!found) {
            int lru_idx = 0;
            int min_time = last_used[0];

            for (k = 0; k < n; k++) {
                if (frames[k] == -1) {
                    lru_idx = k;
                    break;
                }
                if (last_used[k] < min_time) {
                    min_time = last_used[k];
                    lru_idx = k;
                }
            }

            frames[lru_idx] = pages[i];
            last_used[lru_idx] = i;
            faults++;
            printf("访问 %d: 缺页 -> ", pages[i]);
        } else {
            printf("访问 %d: 命中 -> ", pages[i]);
        }

        printf("物理块: ");
        for (j = 0; j < n; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("空 ");
        }
        printf("\n");
    }

    printf("\n缺页次数: %d\n", faults);
    printf("缺页率: %.2f%%\n", (float)faults / m * 100);

    return 0;
}
