#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <string>
#include <mutex>
#include <ctime>
#include <iomanip>

using namespace std;

// Структура для хранения данных о тренировке
struct Training {
    int day;      // день (1-28)
    int month;    // месяц (1-12)
    int year;     // год
    int hour;     // час (6-22)
    int minute;   // минута (0-59)
    string trainerName; // ФИО тренера
};

// Вспомогательная функция для определения дня недели
int getDayOfWeek(int day, int month, int year) {
    tm time_in = {0};
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = day;
    mktime(&time_in);
    return time_in.tm_wday; // 0 - воскресенье 1 - понедельник и тд
}

// Список возможных имен тренеров
vector<string> trainerNames = {
    "Иванов Иван Иванович",
    "Петров Петр Петрович",
    "Сидоров Сидор Сидорович",
    "Козлова Анна Дмитриевна",
    "Смирнов Алексей Владимирович",
    "Васильева Мария Сергеевна",
    "Николаев Дмитрий Александрович",
    "Федорова Елена Ивановна"
};

// Названия дней недели
vector<string> dayNames = {
    "Воскресенье", "Понедельник", "Вторник", "Среда", 
    "Четверг", "Пятница", "Суббота"
};

// Генерация случайных тренировок
vector<Training> generateTrainings(int size) {
    vector<Training> trainings(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dayDist(1, 28);
    uniform_int_distribution<> monthDist(1, 12);
    uniform_int_distribution<> yearDist(2024, 2025);
    uniform_int_distribution<> hourDist(6, 22);
    uniform_int_distribution<> minuteDist(0, 59);
    uniform_int_distribution<> trainerDist(0, trainerNames.size() - 1);

    for (int i = 0; i < size; i++) {
        trainings[i].day = dayDist(gen);
        trainings[i].month = monthDist(gen);
        trainings[i].year = yearDist(gen);
        trainings[i].hour = hourDist(gen);
        trainings[i].minute = minuteDist(gen);
        trainings[i].trainerName = trainerNames[trainerDist(gen)];
    }

    return trainings;
}

// Однопоточный поиск тренировок по дню недели
vector<Training> findTrainingsSingleThread(const vector<Training>& trainings, int targetDayOfWeek) {
    vector<Training> result;
    
    for (const auto& training : trainings) {
        int dayOfWeek = getDayOfWeek(training.day, training.month, training.year);
        if (dayOfWeek == targetDayOfWeek) {
            result.push_back(training);
        }
    }
    
    return result;
}

// Многопоточный поиск тренировок по дню недели
vector<Training> findTrainingsMultiThread(const vector<Training>& trainings, int targetDayOfWeek, int numThreads) {
    vector<Training> result;
    mutex resultMutex;
    
    auto worker = [&](int start, int end) {
        vector<Training> localResult;
        for (int i = start; i < end; i++) {
            int dayOfWeek = getDayOfWeek(trainings[i].day, trainings[i].month, trainings[i].year);
            if (dayOfWeek == targetDayOfWeek) {
                localResult.push_back(trainings[i]);
            }
        }
        
        lock_guard<mutex> lock(resultMutex);
        result.insert(result.end(), localResult.begin(), localResult.end());
    };
    
    vector<thread> threads;
    int chunkSize = trainings.size() / numThreads;
    
    for (int i = 0; i < numThreads; i++) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? trainings.size() : (i + 1) * chunkSize;
        threads.emplace_back(worker, start, end);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    return result;
}

// Вывод информации о тренировке
void printTraining(const Training& t) {
    int dayOfWeek = getDayOfWeek(t.day, t.month, t.year);
    cout << "  " << setfill('0') << setw(2) << t.day << "." 
         << setw(2) << t.month << "." << t.year 
         << " (" << dayNames[dayOfWeek] << ") "
         << setw(2) << t.hour << ":" << setw(2) << t.minute
         << " - " << t.trainerName << endl;
}

int main() {
    // Параметры
    int arraySize;
    int numThreads;
    int targetDayOfWeek;
    
    cout << "=== Многопоточная обработка данных о тренировках ===" << endl;
    
    cout << "Введите размер массива данных: ";
    cin >> arraySize;
    
    cout << "Введите количество потоков: ";
    cin >> numThreads;
    
    cout << "Введите день недели (0-Воскресенье, 1-Понедельник, ..., 6-Суббота): ";
    cin >> targetDayOfWeek;
    
    if (targetDayOfWeek < 0 || targetDayOfWeek > 6) {
        cout << "Ошибка: неверный день недели!" << endl;
        return 1;
    }
    
    cout << "\nИщем тренировки в день: " << dayNames[targetDayOfWeek] << endl;
    cout << "Генерация данных..." << endl;
    
    // Генерация данных
    vector<Training> trainings = generateTrainings(arraySize);
    
    cout << "\n--- ОДНОПОТОЧНАЯ ОБРАБОТКА ---" << endl;
    auto start1 = chrono::high_resolution_clock::now();
    vector<Training> result1 = findTrainingsSingleThread(trainings, targetDayOfWeek);
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration1 = end1 - start1;
    
    cout << "Время выполнения: " << duration1.count() << " мс" << endl;
    cout << "Найдено тренировок: " << result1.size() << endl;
    
    cout << "\n--- МНОГОПОТОЧНАЯ ОБРАБОТКА (потоков: " << numThreads << ") ---" << endl;
    auto start2 = chrono::high_resolution_clock::now();
    vector<Training> result2 = findTrainingsMultiThread(trainings, targetDayOfWeek, numThreads);
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration2 = end2 - start2;
    
    cout << "Время выполнения: " << duration2.count() << " мс" << endl;
    cout << "Найдено тренировок: " << result2.size() << endl;
    
    // Анализ ускорения
    cout << "\n--- АНАЛИЗ ---" << endl;
    cout << "Ускорение: " << fixed << setprecision(2) 
         << (duration1.count() / duration2.count()) << "x" << endl;
    cout << "Разница во времени: " << fixed << setprecision(3)
         << (duration1.count() - duration2.count()) << " мс" << endl;
    
    // Вывод первых 10 результатов
    if (!result1.empty()) {
        cout << "\n--- ПЕРВЫЕ 10 НАЙДЕННЫХ ТРЕНИРОВОК ---" << endl;
        int limit = min(10, (int)result1.size());
        for (int i = 0; i < limit; i++) {
            printTraining(result1[i]);
        }
    } else {
        cout << "\nТренировки не найдены." << endl;
    }
    
    return 0;
}
