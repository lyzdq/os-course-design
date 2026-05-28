#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t chopsticks[N];
pthread_mutex_t mutex;

void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % N;

    for (int i = 0; i < 2; i++) {
        printf("哲学家%d 正在思考\n", id);
        usleep(500000);

        pthread_mutex_lock(&mutex);
        sem_wait(&chopsticks[left]);
        sem_wait(&chopsticks[right]);
        pthread_mutex_unlock(&mutex);

        printf("哲学家%d 正在进餐 (拿起筷子%d和%d)\n", id, left, right);
        usleep(1000000);

        sem_post(&chopsticks[left]);
        sem_post(&chopsticks[right]);
        printf("哲学家%d 放下筷子，开始思考\n", id);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int ids[N] = {0, 1, 2, 3, 4};

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < N; i++)
        sem_init(&chopsticks[i], 0, 1);

    for (int i = 0; i < N; i++)
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);

    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < N; i++)
        sem_destroy(&chopsticks[i]);

    printf("哲学家进餐运行结束\n");
    return 0;
}
