#include "hashMapChain.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;


int main() {

    cout << "\nA program for determining the shortest and longest chain" << endl;;

    srand(time(nullptr));
    vector<int> arr = {5000, 10000, 20000};
    int size;
    for (const int& N : arr) {
        cout << "\nN = " << N << endl;
        size = N / 100;
        HashMapChain hashTable(size);
        for (int i = 0; i <N; i++) {
            int num = rand();
            hashTable.insert(num);
        }

        int minLen, maxLen;
        hashTable.getChainLengths(minLen, maxLen);
        cout << "The shortest chain: " << minLen << endl;
        cout << "The biggest chain: " << maxLen << endl;
        cout << endl;
    }

    return 0;
}