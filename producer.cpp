#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore.h>

using namespace std;

const int BUFFER_SIZE = 5;

vector<int> buffer(BUFFER_SIZE);
sem_t empty, full;
mutex mtx;

void produce(int item) {
    sem_wait(&empty);
    mtx.lock();

    // Critical section (add item to the buffer)
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (buffer[i] == 0) {
            buffer[i] = item;
            break;
        }
    }

    mtx.unlock();
    sem_post(&full);
}

int consume() {
    sem_wait(&full);
    mtx.lock();

    
    int item = 0;
    for (int i = BUFFER_SIZE - 1; i >= 0; --i) {
        if (buffer[i] != 0) {
            item = buffer[i];
            buffer[i] = 0;
            break;
        }
    }

    mtx.unlock();
    sem_post(&empty);

    return item;
}

void producer(int items) {
    for (int i = 1; i <= items; ++i) {
        produce(i);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void consumer(int items) {
    for (int i = 1; i <= items; ++i) {
        int item = consume();
        cout << "Consumed: " << item << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

int main() {
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    int producerItems, consumerItems;

    cout << "Enter the number of items to be produced: ";
    cin >> producerItems;

    cout << "Enter the number of items to be consumed: ";
    cin >> consumerItems;

    thread producerThread(producer, producerItems);
    thread consumerThread(consumer, consumerItems);

    producerThread.join();
    consumerThread.join();

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
