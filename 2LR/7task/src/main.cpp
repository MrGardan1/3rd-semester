#include "LRUCache.h"

#include <string>
#include <sstream>
#include <iostream>
#include <limits>
using namespace std;

int main() {
    
    cout << "\nA program that implements the LRU cache" << endl;

    cout << "\nEnter the LRU-cache capacity" << endl;

    int cap;
    int req;
    try {
        if (!(cin >> cap)) {
            throw invalid_argument("capacity must be integer");
        }
        if (cap <= 0) {
            throw invalid_argument("capacity must be >0");
        }

        cout << "Enter the number of requests" << endl;

        if (!(cin >> req)) {
            throw invalid_argument("requests must be integer");
        }
        if (req <= 0) {
            throw invalid_argument("requests must be >0");
        }
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        LRUCache lru(cap);
        cout << "Enter the queries (1 query at a time): ";

        string line;
        for (int i = 0; i < req; i++) {
            
            getline(cin, line);
            istringstream iss (line);
            string command;
            iss >> command;
            int key = 0;
            int value = 0;
            if (command == "GET") {
                if (!(iss >> key)) {
                    throw invalid_argument("key must be integer");
                }
                if (key < 0) {
                    throw invalid_argument("key must be >=0");
                }
                int res = lru.get(key);
                cout << res;

            } else if (command == "SET") {

                if (!(iss >> key)) {
                    throw invalid_argument("key must be integer");
                }
                if (key < 0) {
                    throw invalid_argument("key must be >=0");
                }
                if (!(iss >> value)) {
                    throw invalid_argument("value must be integer");
                }
                lru.set(key, value);

            } else {
                cout << "Error: unknown command" << endl;
                i--;
            }
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    } catch (const runtime_error &e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    return 0;
}