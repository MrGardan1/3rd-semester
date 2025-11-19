#include "tree.h"
#include <limits>
#include <iostream>
using namespace std;

int main() {

    cout << "\nA program for checking for an AVL tree" << endl;

    cout << "Enter sequence of integers ending with 0 (0 not included):" << endl;
    Tree tree;
    int x;

    while (true) {
        cout << "Next number: ";
        
        if (!(cin >> x)) {
            cout << "Invalid argument, must be integer." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (x == 0) break;

        tree.insert(x);
    }

    cout << (tree.isAVL() ? "YES" : "NO") << endl;

    return 0;
}