#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 3
#define PROD_COUNT 2
#define CONS_COUNT 2
#define ITEMS_PER_THREAD 4

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = id * 100 + i;
        printf("生产者%d 生产了物品%d (缓冲区位置:%d, 当前占用:%d/%d)\n",
               id, id * 100 + i, in, (in - out + BUFFER_SIZE) % BUFFER_SIZE + 1, BUFFER_SIZE);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        usleep(500000);  // 生产快，0.5秒
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("消费者%d 消费了物品%d (缓冲区位置:%d, 当前占用:%d/%d)\n",
               id, item, out, (in - out + BUFFER_SIZE) % BUFFER_SIZE, BUFFER_SIZE);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        usleep(1500000);  // 消费慢，1.5秒
    }
    return NULL;
}

int main() {
    pthread_t prod[PROD_COUNT], cons[CONS_COUNT];
    int prod_id[PROD_COUNT] = {1, 2};
    int cons_id[CONS_COUNT] = {1, 2};

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < PROD_COUNT; i++)
        pthread_create(&prod[i], NULL, producer, &prod_id[i]);
    for (int i = 0; i < CONS_COUNT; i++)
        pthread_create(&cons[i], NULL, consumer, &cons_id[i]);

    for (int i = 0; i < PROD_COUNT; i++)
        pthread_join(prod[i], NULL);
    for (int i = 0; i < CONS_COUNT; i++)
        pthread_join(cons[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("生产者-消费者运行结束\n");
    return 0;
}
