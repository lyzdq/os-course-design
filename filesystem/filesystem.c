#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILES 20
#define DISK_SIZE 1024
#define BLOCK_SIZE 64
#define MAX_BLOCKS 16
#define DATA_PER_BLOCK 60

typedef struct {
    char name[32];
    int size;
    int start_block;
    bool used;
} File;

typedef struct {
    bool allocated;
    int next;
    char data[DATA_PER_BLOCK];
} Block;

File files[MAX_FILES];
Block disk[MAX_BLOCKS];

int free_space;
int file_count;

void init_fs() {
    free_space = DISK_SIZE;
    file_count = 0;
    for (int i = 0; i < MAX_FILES; i++) files[i].used = false;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        disk[i].allocated = false;
        memset(disk[i].data, 0, DATA_PER_BLOCK);
    }
    printf("文件系统初始化完成，总空间:%d\n", DISK_SIZE);
}

int find_file(char name[]) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0)
            return i;
    }
    return -1;
}

void create_file(char name[], int size) {
    if (file_count >= MAX_FILES) {
        printf("文件数已满\n"); return;
    }
    if (find_file(name) != -1) {
        printf("文件 %s 已存在\n", name); return;
    }
    if (size > free_space) {
        printf("空间不足\n"); return;
    }
    int blocks_needed = (size + DATA_PER_BLOCK - 1) / DATA_PER_BLOCK;
    int blocks[MAX_BLOCKS], count = 0;

    for (int i = 0; i < MAX_BLOCKS && count < blocks_needed; i++) {
        if (!disk[i].allocated) blocks[count++] = i;
    }
    if (count < blocks_needed) {
        printf("磁盘块不足\n"); return;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {
            strcpy(files[i].name, name);
            files[i].size = size;
            files[i].start_block = blocks[0];
            files[i].used = true;
            for (int j = 0; j < blocks_needed; j++) {
                disk[blocks[j]].allocated = true;
                disk[blocks[j]].next = (j < blocks_needed - 1) ? blocks[j + 1] : -1;
                memset(disk[blocks[j]].data, 0, DATA_PER_BLOCK);
            }
            file_count++;
            free_space -= blocks_needed * BLOCK_SIZE;
            printf("文件 %s 创建成功 (大小:%d, 块数:%d)\n", name, size, blocks_needed);
            return;
        }
    }
}

void read_file(char name[]) {
    int idx = find_file(name);
    if (idx == -1) {
        printf("文件 %s 不存在\n", name); return;
    }
    printf("===== 文件 %s 的内容 =====\n", name);
    int block = files[idx].start_block;
    while (block != -1) {
        printf("%s", disk[block].data);
        block = disk[block].next;
    }
    printf("\n===== 读取完毕 =====\n");
}

void write_file(char name[]) {
    int idx = find_file(name);
    if (idx == -1) {
        printf("文件 %s 不存在\n", name); return;
    }
    printf("请输入要写入的内容(最大%d字符): ", files[idx].size);
    getchar();
    char input[1024];
    fgets(input, 1024, stdin);
    input[strcspn(input, "\n")] = '\0';

    int total = strlen(input);
    if (total > files[idx].size) {
        printf("内容超过文件大小，将被截断\n");
        total = files[idx].size;
    }

    int written = 0, block = files[idx].start_block;
    while (block != -1 && written < total) {
        int to_copy = total - written;
        if (to_copy > DATA_PER_BLOCK) to_copy = DATA_PER_BLOCK;
        strncpy(disk[block].data, input + written, to_copy);
        disk[block].data[to_copy] = '\0';
        written += to_copy;
        block = disk[block].next;
    }
    printf("写入完成，共写入 %d 字节\n", written);
}

void delete_file(char name[]) {
    int idx = find_file(name);
    if (idx == -1) {
        printf("文件 %s 不存在\n", name); return;
    }
    int block = files[idx].start_block;
    int count = 0;
    while (block != -1) {
        int next = disk[block].next;
        disk[block].allocated = false;
        memset(disk[block].data, 0, DATA_PER_BLOCK);
        block = next;
        count++;
    }
    free_space += count * BLOCK_SIZE;
    files[idx].used = false;
    file_count--;
    printf("文件 %s 已删除，释放 %d 块\n", name, count);
}

void list_files() {
    printf("\n===== 文件列表 =====\n");
    printf("文件名\t大小\t起始块\n");
    int count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            printf("%s\t%d\t%d\n", files[i].name, files[i].size, files[i].start_block);
            count++;
        }
    }
    if (count == 0) printf("(无文件)\n");
    printf("剩余空间: %d/%d\n", free_space, DISK_SIZE);
}

void show_blocks() {
    printf("\n===== 磁盘块状态 =====\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        printf("块%d: %s", i, disk[i].allocated ? "已分配" : "空闲");
        if (disk[i].allocated) printf(" -> %d 数据:[%s]", disk[i].next, disk[i].data);
        printf("\n");
    }
}

int main() {
    int choice, size;
    char name[32];
    init_fs();

    while (1) {
        printf("\n1.创建  2.读取  3.写入  4.删除  5.列表  6.磁盘  0.退出\n选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("文件名和大小: ");
                scanf("%s %d", name, &size);
                create_file(name, size);
                break;
            case 2:
                printf("文件名: ");
                scanf("%s", name);
                read_file(name);
                break;
            case 3:
                printf("文件名: ");
                scanf("%s", name);
                write_file(name);
                break;
            case 4:
                printf("文件名: ");
                scanf("%s", name);
                delete_file(name);
                break;
            case 5: list_files(); break;
            case 6: show_blocks(); break;
            case 0: return 0;
        }
    }
}
