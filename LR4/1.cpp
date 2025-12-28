#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>
#include <random>
#include <atomic>
#include <condition_variable>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

// ============= ПРИМИТИВЫ СИНХРОНИЗАЦИИ =============

// 1. Mutex (обычный мьютекс)
class MutexTest {
private:
    mutex mtx;
    string result;
public:
    void addChar(char c) {
        lock_guard<mutex> lock(mtx);
        result += c;
    }
    string getResult() { return result; }
};

// 2. Semaphore (реализация через condition_variable)
class Semaphore {
private:
    mutex mtx;
    condition_variable cv;
    int count;
public:
    Semaphore(int count = 1) : count(count) {}
    
    void acquire() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return count > 0; });
        count--;
    }
    
    void release() {
        lock_guard<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
};

class SemaphoreTest {
private:
    Semaphore sem;
    string result;
public:
    SemaphoreTest() : sem(1) {}
    
    void addChar(char c) {
        sem.acquire();
        result += c;
        sem.release();
    }
    string getResult() { return result; }
};

// 3. Barrier (C++20, для C++17 используем свою реализацию)
class Barrier {
private:
    mutex mtx;
    condition_variable cv;
    int count;
    int waiting;
    int generation;
public:
    Barrier(int count) : count(count), waiting(0), generation(0) {}
    
    void wait() {
        unique_lock<mutex> lock(mtx);
        int gen = generation;
        if (++waiting == count) {
            waiting = 0;
            generation++;
            cv.notify_all();
        } else {
            cv.wait(lock, [this, gen]() { return gen != generation; });
        }
    }
};

class BarrierTest {
private:
    Barrier barrier;
    mutex mtx;
    string result;
public:
    BarrierTest(int numThreads) : barrier(numThreads) {}
    
    void addChar(char c) {
        barrier.wait();
        lock_guard<mutex> lock(mtx);
        result += c;
    }
    string getResult() { return result; }
};

// 4. SpinLock (активное ожидание)
class SpinLock {
private:
    atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(memory_order_acquire)) {
            // Активное ожидание
        }
    }
    
    void unlock() {
        flag.clear(memory_order_release);
    }
};

class SpinLockTest {
private:
    SpinLock spinlock;
    string result;
public:
    void addChar(char c) {
        spinlock.lock();
        result += c;
        spinlock.unlock();
    }
    string getResult() { return result; }
};

// 5. SpinWait (активное ожидание с паузами)
class SpinWait {
private:
    atomic<bool> locked{false};
public:
    void lock() {
        while (locked.exchange(true, memory_order_acquire)) {
            while (locked.load(memory_order_relaxed)) {
                this_thread::yield(); // Уступаем процессор
            }
        }
    }
    
    void unlock() {
        locked.store(false, memory_order_release);
    }
};

class SpinWaitTest {
private:
    SpinWait spinwait;
    string result;
public:
    void addChar(char c) {
        spinwait.lock();
        result += c;
        spinwait.unlock();
    }
    string getResult() { return result; }
};

// 6. Monitor (паттерн монитора)
class Monitor {
private:
    mutex mtx;
    condition_variable cv;
    string result;
public:
    void addChar(char c) {
        unique_lock<mutex> lock(mtx);
        result += c;
        cv.notify_all();
    }
    string getResult() { 
        unique_lock<mutex> lock(mtx);
        return result; 
    }
};

// ============= ГЕНЕРАТОР СЛУЧАЙНЫХ СИМВОЛОВ =============
char getRandomAsciiChar() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(33, 126); // Печатные ASCII символы
    return static_cast<char>(dis(gen));
}

// ============= ФУНКЦИИ ТЕСТИРОВАНИЯ =============

template<typename T>
double testPrimitive(T& primitive, int numThreads, int charsPerThread, const string& name) {
    vector<thread> threads;
    
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&primitive, charsPerThread]() {
            for (int j = 0; j < charsPerThread; j++) {
                char c = getRandomAsciiChar();
                primitive.addChar(c);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = high_resolution_clock::now();
    duration<double, milli> elapsed = end - start;
    
    cout << left << setw(20) << name 
         << "| Время: " << setw(10) << fixed << setprecision(3) << elapsed.count() << " мс"
         << " | Символов: " << primitive.getResult().length() << endl;
    
    return elapsed.count();
}

// Специальный тест для Monitor
double testMonitor(Monitor& monitor, int numThreads, int charsPerThread) {
    vector<thread> threads;
    
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&monitor, charsPerThread]() {
            for (int j = 0; j < charsPerThread; j++) {
                char c = getRandomAsciiChar();
                monitor.addChar(c);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = high_resolution_clock::now();
    duration<double, milli> elapsed = end - start;
    
    cout << left << setw(20) << "Monitor" 
         << "| Время: " << setw(10) << fixed << setprecision(3) << elapsed.count() << " мс"
         << " | Символов: " << monitor.getResult().length() << endl;
    
    return elapsed.count();
}

// ============= MAIN =============
int main() {
    int numThreads;
    int charsPerThread;
    
    cout << "=== ЗАДАНИЕ 1: Примитивы синхронизации ===" << endl << endl;
    
    cout << "Введите количество потоков: ";
    cin >> numThreads;
    
    cout << "Введите количество символов на поток: ";
    cin >> charsPerThread;
    
    cout << "\n--- ЗАПУСК ТЕСТИРОВАНИЯ ---" << endl;
    cout << "Общее количество операций: " << (numThreads * charsPerThread) << endl << endl;
    
    vector<pair<string, double>> results;
    
    // Тест 1: Mutex
    {
        MutexTest test;
        double time = testPrimitive(test, numThreads, charsPerThread, "Mutex");
        results.push_back({"Mutex", time});
    }
    
    // Тест 2: Semaphore
    {
        SemaphoreTest test;
        double time = testPrimitive(test, numThreads, charsPerThread, "Semaphore");
        results.push_back({"Semaphore", time});
    }
    
    // Тест 3: Barrier
    {
        BarrierTest test(numThreads);
        double time = testPrimitive(test, numThreads, charsPerThread, "Barrier");
        results.push_back({"Barrier", time});
    }
    
    // Тест 4: SpinLock
    {
        SpinLockTest test;
        double time = testPrimitive(test, numThreads, charsPerThread, "SpinLock");
        results.push_back({"SpinLock", time});
    }
    
    // Тест 5: SpinWait
    {
        SpinWaitTest test;
        double time = testPrimitive(test, numThreads, charsPerThread, "SpinWait");
        results.push_back({"SpinWait", time});
    }
    
    // Тест 6: Monitor
    {
        Monitor test;
        double time = testMonitor(test, numThreads, charsPerThread);
        results.push_back({"Monitor", time});
    }
    
    // Сортировка результатов
    sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    cout << "\n=== РЕЙТИНГ ПО СКОРОСТИ ===" << endl;
    for (int i = 0; i < results.size(); i++) {
        cout << (i + 1) << ". " << left << setw(15) << results[i].first 
             << " - " << fixed << setprecision(3) << results[i].second << " мс" << endl;
    }
    
    cout << "\n=== АНАЛИЗ ===" << endl;
    cout << "Самый быстрый: " << results[0].first << endl;
    cout << "Самый медленный: " << results.back().first << endl;
    cout << "Разница: " << fixed << setprecision(2) 
         << (results.back().second / results[0].second) << "x" << endl;
    
    return 0;
}
