#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READER_COUNT 3
#define WRITER_COUNT 2

int shared_data = 0;
int read_count = 0;

pthread_mutex_t mutex;
pthread_mutex_t rw_mutex;

void* reader(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 3; i++) {
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1)
            pthread_mutex_lock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

        printf("读者%d 读取数据: %d (当前读者数:%d)\n", id, shared_data, read_count);
        usleep(500000);

        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0)
            pthread_mutex_unlock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

        usleep(1000000);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 2; i++) {
        pthread_mutex_lock(&rw_mutex);

        shared_data++;
        printf("写者%d 写入数据: %d\n", id, shared_data);
        usleep(800000);

        pthread_mutex_unlock(&rw_mutex);
        usleep(1500000);
    }
    return NULL;
}

int main() {
    pthread_t readers[READER_COUNT], writers[WRITER_COUNT];
    int reader_id[READER_COUNT] = {1, 2, 3};
    int writer_id[WRITER_COUNT] = {1, 2};

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&rw_mutex, NULL);

    for (int i = 0; i < READER_COUNT; i++)
        pthread_create(&readers[i], NULL, reader, &reader_id[i]);
    for (int i = 0; i < WRITER_COUNT; i++)
        pthread_create(&writers[i], NULL, writer, &writer_id[i]);

    for (int i = 0; i < READER_COUNT; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < WRITER_COUNT; i++)
        pthread_join(writers[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&rw_mutex);

    printf("读者-写者运行结束\n");
    return 0;
}
