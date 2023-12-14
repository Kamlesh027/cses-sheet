#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;  // Add the using namespace std directive

std::mutex mutex1, mutex2;

void deadlockThread1() {
    lock_guard<mutex> lock1(mutex1);
    this_thread::sleep_for(chrono::milliseconds(100));  // Simulating some work
    lock_guard<mutex> lock2(mutex2);
    cout << "Thread 1 acquired both locks.\n";
}

void deadlockThread2() {
    lock_guard<mutex> lock2(mutex2);
    this_thread::sleep_for(chrono::milliseconds(100));  // Simulating some work
    lock_guard<mutex> lock1(mutex1);
    cout << "Thread 2 acquired both locks.\n";
}

int main() {
    thread t1(deadlockThread1);
    thread t2(deadlockThread2);

    t1.join();
    t2.join();

    return 0;
}
 