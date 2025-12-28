#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

// ============= БАЗА ДАННЫХ =============
class Database {
private:
    vector<int> data;
    mutable mutex mtx;
    
public:
    Database() {
        // Инициализация данными
        for (int i = 0; i < 100; i++) {
            data.push_back(i);
        }
    }
    
    int read(int index) const {
        lock_guard<mutex> lock(mtx);
        if (index >= 0 && index < data.size()) {
            return data[index];
        }
        return -1;
    }
    
    void write(int index, int value) {
        lock_guard<mutex> lock(mtx);
        if (index >= 0 && index < data.size()) {
            data[index] = value;
        }
    }
    
    int size() const {
        lock_guard<mutex> lock(mtx);
        return data.size();
    }
};

// ============= ПРИОРИТЕТ ЧИТАТЕЛЕЙ =============
class ReaderPriorityRW {
private:
    Database& db;
    mutable mutex mtx;
    condition_variable cv;
    int readers = 0;
    int writers = 0;
    int writersWaiting = 0;
    
public:
    ReaderPriorityRW(Database& database) : db(database) {}
    
    void startRead() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return writers == 0; });
        readers++;
    }
    
    void endRead() {
        unique_lock<mutex> lock(mtx);
        readers--;
        if (readers == 0) {
            cv.notify_all();
        }
    }
    
    void startWrite() {
        unique_lock<mutex> lock(mtx);
        writersWaiting++;
        cv.wait(lock, [this]() { return readers == 0 && writers == 0; });
        writersWaiting--;
        writers++;
    }
    
    void endWrite() {
        unique_lock<mutex> lock(mtx);
        writers--;
        cv.notify_all();
    }
    
    int read(int index) {
        startRead();
        int value = db.read(index);
        this_thread::sleep_for(milliseconds(1)); // Имитация чтения
        endRead();
        return value;
    }
    
    void write(int index, int value) {
        startWrite();
        db.write(index, value);
        this_thread::sleep_for(milliseconds(2)); // Имитация записи
        endWrite();
    }
};

// ============= РЕШЕНИЕ 2: ПРИОРИТЕТ ПИСАТЕЛЕЙ =============
class WriterPriorityRW {
private:
    Database& db;
    mutable mutex mtx;
    condition_variable cvReaders;
    condition_variable cvWriters;
    int readers = 0;
    int writers = 0;
    int writersWaiting = 0;
    
public:
    WriterPriorityRW(Database& database) : db(database) {}
    
    void startRead() {
        unique_lock<mutex> lock(mtx);
        cvReaders.wait(lock, [this]() { 
            return writers == 0 && writersWaiting == 0; 
        });
        readers++;
    }
    
    void endRead() {
        unique_lock<mutex> lock(mtx);
        readers--;
        if (readers == 0) {
            cvWriters.notify_one();
        }
    }
    
    void startWrite() {
        unique_lock<mutex> lock(mtx);
        writersWaiting++;
        cvWriters.wait(lock, [this]() { return readers == 0 && writers == 0; });
        writersWaiting--;
        writers++;
    }
    
    void endWrite() {
        unique_lock<mutex> lock(mtx);
        writers--;
        if (writersWaiting > 0) {
            cvWriters.notify_one();
        } else {
            cvReaders.notify_all();
        }
    }
    
    int read(int index) {
        startRead();
        int value = db.read(index);
        this_thread::sleep_for(milliseconds(1));
        endRead();
        return value;
    }
    
    void write(int index, int value) {
        startWrite();
        db.write(index, value);
        this_thread::sleep_for(milliseconds(2));
        endWrite();
    }
};

// ============= РЕШЕНИЕ 3: СПРАВЕДЛИВОЕ (shared_mutex) =============
class FairRW {
private:
    Database& db;
    mutable shared_mutex smtx;
    
public:
    FairRW(Database& database) : db(database) {}
    
    int read(int index) {
        shared_lock<shared_mutex> lock(smtx);
        int value = db.read(index);
        this_thread::sleep_for(milliseconds(1));
        return value;
    }
    
    void write(int index, int value) {
        unique_lock<shared_mutex> lock(smtx);
        db.write(index, value);
        this_thread::sleep_for(milliseconds(2));
    }
};

// ============= СТАТИСТИКА =============
struct Stats {
    int readsCompleted = 0;
    int writesCompleted = 0;
    mutex mtx;
    
    void addRead() {
        lock_guard<mutex> lock(mtx);
        readsCompleted++;
    }
    
    void addWrite() {
        lock_guard<mutex> lock(mtx);
        writesCompleted++;
    }
};

// ============= ФУНКЦИИ ТЕСТИРОВАНИЯ =============
template<typename RW>
void runTest(RW& rw, int numReaders, int numWriters, int operations, Stats& stats) {
    vector<thread> threads;
    random_device rd;
    
    // Создание читателей
    for (int i = 0; i < numReaders; i++) {
        threads.emplace_back([&rw, &stats, operations, seed = rd()]() {
            mt19937 gen(seed);
            uniform_int_distribution<> dis(0, 99);
            
            for (int j = 0; j < operations; j++) {
                int index = dis(gen);
                rw.read(index);
                stats.addRead();
            }
        });
    }
    
    // Создание писателей
    for (int i = 0; i < numWriters; i++) {
        threads.emplace_back([&rw, &stats, operations, seed = rd()]() {
            mt19937 gen(seed);
            uniform_int_distribution<> dis(0, 99);
            uniform_int_distribution<> valDis(1000, 9999);
            
            for (int j = 0; j < operations; j++) {
                int index = dis(gen);
                int value = valDis(gen);
                rw.write(index, value);
                stats.addWrite();
            }
        });
    }
    
    // Ожидание завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }
}

// ============= MAIN =============
int main() {
    int numReaders, numWriters, operations;
    
    cout << "=== ЗАДАНИЕ 3: Задача Читатели-Писатели ===" << endl << endl;
    
    cout << "Введите количество читателей: ";
    cin >> numReaders;
    
    cout << "Введите количество писателей: ";
    cin >> numWriters;
    
    cout << "Введите количество операций на поток: ";
    cin >> operations;
    
    cout << "\n=== ТЕСТИРОВАНИЕ СТРАТЕГИЙ ===" << endl << endl;
    
    // Тест 1: Приоритет читателей
    {
        cout << "1. ПРИОРИТЕТ ЧИТАТЕЛЕЙ" << endl;
        Database db;
        ReaderPriorityRW rw(db);
        Stats stats;
        
        auto start = high_resolution_clock::now();
        runTest(rw, numReaders, numWriters, operations, stats);
        auto end = high_resolution_clock::now();
        
        duration<double, milli> elapsed = end - start;
        
        cout << "   Время выполнения: " << fixed << setprecision(2) << elapsed.count() << " мс" << endl;
        cout << "   Операций чтения: " << stats.readsCompleted << endl;
        cout << "   Операций записи: " << stats.writesCompleted << endl;
        cout << "   Пропускная способность: " << fixed << setprecision(0) 
             << ((stats.readsCompleted + stats.writesCompleted) / (elapsed.count() / 1000.0)) 
             << " оп/сек" << endl << endl;
    }
    
    // Тест 2: Приоритет писателей
    {
        cout << "2. ПРИОРИТЕТ ПИСАТЕЛЕЙ" << endl;
        Database db;
        WriterPriorityRW rw(db);
        Stats stats;
        
        auto start = high_resolution_clock::now();
        runTest(rw, numReaders, numWriters, operations, stats);
        auto end = high_resolution_clock::now();
        
        duration<double, milli> elapsed = end - start;
        
        cout << "   Время выполнения: " << fixed << setprecision(2) << elapsed.count() << " мс" << endl;
        cout << "   Операций чтения: " << stats.readsCompleted << endl;
        cout << "   Операций записи: " << stats.writesCompleted << endl;
        cout << "   Пропускная способность: " << fixed << setprecision(0) 
             << ((stats.readsCompleted + stats.writesCompleted) / (elapsed.count() / 1000.0)) 
             << " оп/сек" << endl << endl;
    }
    
    // Тест 3: Справедливая стратегия
    {
        cout << "3. СПРАВЕДЛИВАЯ СТРАТЕГИЯ (shared_mutex)" << endl;
        Database db;
        FairRW rw(db);
        Stats stats;
        
        auto start = high_resolution_clock::now();
        runTest(rw, numReaders, numWriters, operations, stats);
        auto end = high_resolution_clock::now();
        
        duration<double, milli> elapsed = end - start;
        
        cout << "   Время выполнения: " << fixed << setprecision(2) << elapsed.count() << " мс" << endl;
        cout << "   Операций чтения: " << stats.readsCompleted << endl;
        cout << "   Операций записи: " << stats.writesCompleted << endl;
        cout << "   Пропускная способность: " << fixed << setprecision(0) 
             << ((stats.readsCompleted + stats.writesCompleted) / (elapsed.count() / 1000.0)) 
             << " оп/сек" << endl << endl;
    }
    
    cout << "=== ВЫВОДЫ ===" << endl;
    cout << "• Приоритет читателей: максимальный параллелизм чтения, риск голодания писателей" << endl;
    cout << "• Приоритет писателей: гарантированная запись, риск голодания читателей" << endl;
    cout << "• Справедливая стратегия: баланс между читателями и писателями" << endl;
    
    return 0;
}
