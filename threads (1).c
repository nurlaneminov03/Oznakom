#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

int durations[NUM_THREADS];

typedef struct {
    int thread_num;
    int sleep_time;
} ThreadData;

void* thread_func(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("%d-й поток начал работу (время %d сек.)\n", data->thread_num + 1, data->sleep_time);
    sleep(data->sleep_time);
    printf("%d-й поток выполнил задачу\n", data->thread_num + 1);
    free(data); 
    return NULL;
}

void run_threads() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Введите время работы для потока %d (в секундах): ", i + 1);
        scanf("%d", &durations[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        ThreadData* data = malloc(sizeof(ThreadData));
        data->thread_num = i;
        data->sleep_time = durations[i];

        if (pthread_create(&threads[i], NULL, thread_func, data) != 0) {
            perror("Ошибка при создании потока");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("\nВсе потоки завершили работу.\n");
}

int main() {
    char choice;

    do {
        run_threads();

        printf("\nНажмите 'x' для выхода, 'r' для перезапуска: ");
        scanf(" %c", &choice);

    } while (choice == 'r' || choice == 'R');

    printf("Программа завершена.\n");
    return 0;
}
