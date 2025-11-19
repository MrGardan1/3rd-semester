#include "testChain.h"
#include "testOpenAdd.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "\nProgram for empirical analysis of chain method and open addressing\n";

    cout << "Enter the number of items: ";
    int N;
    int M;
    try {
        if (!(cin >> N)) {
            throw invalid_argument("number of items must be integer");
        }
        if (N <= 0) {
            throw invalid_argument("number of items must be >0");
        }
        cout << "Enter the number of searches: ";
        if (!(cin >> M)) {
            throw invalid_argument("number of searches must be integer");
        }
        if (M <= 0) {
            throw invalid_argument("number of searches must be >0");
        }
    }
    catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    int tableSize = N;
    chainHashMap chainHash(tableSize / 10);
    OpenAddHM openHash(tableSize);

    vector<pair<int,int>> keys;
    keys.reserve(N);

    for (int i = 0; i < N; i++) {
        int key = rand();
        int value = rand();
        keys.emplace_back(key, value);
        chainHash.insert(key, value);
        openHash.insert(key, value);
    }

    auto startChain = high_resolution_clock::now();
    int foundChain = 0;
    for (int i = 0; i < M; i++) {
        int key = keys[rand() % N].first;
        int val;
        if (chainHash.search(key) != -1) {
            foundChain++;
        }
    }
    auto endChain = high_resolution_clock::now();
    auto chainDuration = duration_cast<milliseconds>(endChain - startChain).count();

    auto startOA = high_resolution_clock::now();
    int foundOA = 0;
    for (int i = 0; i < M; i++) {
        int key = keys[rand() % N].first;
        int val;
        if (openHash.search(key) != -1) {
            foundOA++;
        }
    }
    auto endOA = high_resolution_clock::now();
    auto openAddDuration = duration_cast<milliseconds>(endOA - startOA).count();

    cout << "Chain method: found " << foundChain << " out of " << M << " searches, time (ms): " << chainDuration << endl;
    cout << "Open addressing method: found " << foundOA << " out of " << M << " searches, time (ms): " << openAddDuration << endl;

    return 0;
}



/*
#include "testChain.h"
#include "testOpenAdd.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;


int main() {
    srand(time(nullptr));

    cout << "\nA program for empirical analysis of the chain method and open addressing" << endl;;

    cout << "Enter the number of items: ";
    int N;
    int M;
    try {
        if (!(cin >> N)) {
            throw invalid_argument("number of items must be integer");
        }
        if (N <= 0) {
            throw invalid_argument("number of items must be >0");
        }

        cout << "Enter the number of search: ";
        if (!(cin >> M)) {
            throw invalid_argument("number of search must be integer");
        }
        if (M<=0) {
            throw invalid_argument("number of search must be >0");
        }

    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    int tableSize = N;
    chainHashMap chainHash(tableSize/10);
    OpenAddHM openHash(tableSize);

    vector<pair<int,int>>keys;
    keys.reserve(N);

    for (int i =0; i < N; i++) {
        int num = rand();
        int value = rand();
        keys.emplace_back(num, value);
        chainHash.insert(num, value);
        openHash.insert(num,value);
    }

    //измерение для метода цепочек
    auto startChain = high_resolution_clock::now();
    int foundChain = 0;
    for (int i = 0; i < M; i++) {
        int randIndex = rand() % N;
        int key = keys[randIndex].first;
        int value = keys[randIndex].second;
        if (chainHash.search(key, value)) {
            foundChain++;
        }
    }
    auto endChain = high_resolution_clock::now();
    auto chainDuration = duration_cast<milliseconds>(endChain-startChain).count();

    //измерение для открытой адресации
    auto startOA = high_resolution_clock::now();
    int foundOA = 0;
    for (int i = 0; i<M; i++) {
        int randIndex = rand() % N;
        int key = keys[randIndex].first;
        int value = keys[randIndex].second;
        if (openHash.search(key, value)) {
            foundOA++;
        }
    }
    auto endOA = high_resolution_clock::now();
    auto openAddDuration = duration_cast<milliseconds>(endOA - startOA).count();
    
    cout << "Chain method: found " << foundChain << " from " << M << " searches, time (ms): " << chainDuration <<  endl;
    cout << "Open addressing method: found " << foundOA << " from " << M << " searches, time (ms): " << openAddDuration << endl;
    
    return 0;
}*/