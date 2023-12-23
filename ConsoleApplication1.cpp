#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool ready = 0;

void* producer(void*)
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        if (ready)
        {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        ready = true;
        std::cout << "Поставщик: Инициировано условное событие" << std::endl;
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return nullptr;
}

void* consumer(void*)
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        while (!ready)
        {
            pthread_cond_wait(&condition, &mutex);
        }
        ready = false;
        std::cout << "Потребитель: Получено условное событие" << std::endl;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main() {
    pthread_t producerThread, consumerThread;

    // Запускаем потоки
    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);

    return 0;
}
