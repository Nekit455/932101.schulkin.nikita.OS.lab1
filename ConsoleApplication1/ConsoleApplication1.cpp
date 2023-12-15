#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class Monitor {
public:
    Monitor() : eventCount(0), maxEventCount(5) {} // реализация для 5 событий

    // функция-поставщик
    void providerThread() {
        while (eventCount < maxEventCount) {
            // Задержка в 1 секунду
            this_thread::sleep_for(chrono::seconds(1));

            {
                unique_lock<mutex> lock(mutex_);
                cout << "Поставщик: Инициировано условное событие.\n";
                eventReady = true;
                eventCount++;
            }

            conditionVariable.notify_one();
        }
    }

    // функция-потребитель
    void consumerThread() {
        while (eventCount < maxEventCount) {
            unique_lock<mutex> lock(mutex_);

            // Ожидание условного события
            conditionVariable.wait(lock, [this] { return eventReady; });

            // Обработка условного события
            cout << "Потребитель: Получено условное событие.\n";
            eventReady = false;
        }
    }

private:
    mutex mutex_;
    condition_variable conditionVariable;
    bool eventReady;
    int eventCount;
    const int maxEventCount;
};

int main() {
    setlocale(LC_ALL, "rus");
    Monitor *monitor = new Monitor();

    // Создание и запуск потоков
    thread provider(&Monitor::providerThread, monitor);
    thread consumer(&Monitor::consumerThread, monitor);

    provider.join();
    consumer.join();

    return 0;
}
