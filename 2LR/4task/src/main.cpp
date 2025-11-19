#include "function.h"
#include <iostream>
#include <string>
#include <limits>
using namespace std;


int main() {

    cout << "\nA program for comparing with a preset template" << endl;
    
    try {

        int count;
        while (true) {
            cout << "Number of comparisons: ";
            if (cin >> count) {
                break;
            } else {
                cout << "Invalid input, count must be integer" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        
        string pattern;
        while (true) {
            cout << "Enter the template: ";
            if (!(cin >> pattern)) {
                cout << "Invalid argument, must be string" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                break;
            }
        }
        
        cout << "\nEnter the lines to compare: " << endl;
        Array arr(count);
        string target;
        for (int i = 0; i < count; i++) {
            while (true) {
                if (cin >> target) {
                    arr.insert(target);
                    break;
                } else {
                    cout << "Invalid argument, must be string" << endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }
        cout << endl;
        
        for (int i= 0; i< count; i++) {
            string element = arr.get(i);
            if (isEquivalent(element, pattern)) {
                cout << element << " matches the template" << endl;
            } else {
                cout << element << " doen't matches the template" << endl;
            }
        }

    } catch (...) {
        cout << "\nError: unknown error"<< endl;
        return -1;
    }



    
    return 0;
}