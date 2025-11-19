#include "set.h"
#include <iostream>
#include <limits>
using namespace std;


int main() {
    
    cout << "\nA program complete a operations with set: unions of set, intersection of set, difference of set" << endl;

    cout << "\nEnter the size (count unique elements) of the first set: ";
    int size1;
    try {
        if (!(cin >> size1) && size1 < 0) {
            throw invalid_argument("size must be integer and positive");
        } else if (size1 < 0) {
            throw runtime_error("size must be >0");
        }

        cout << "Enter the size (count unique elements) of the second set: ";
        int size2;

        if ( !(cin >> size2)) {
            throw invalid_argument("size must be integer and positive");
        } else if (size2 < 0) {
            throw runtime_error("size must be >0");
        }
        
        Set set1(size1);
        int element;
        int inserted = 0;
        cout << "\nEnter the elements for first set: " << endl;

        while (inserted < size1) {
            if (!(cin >> element)) {
                cout << "Error: element must be integer" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                if (!set1.contains(element)) {
                    set1.insert(element);
                    inserted++;
                } else {
                    cout << "Doublicate element, enter another value" << endl;
                }
            }
        }

        Set set2(size2);
        inserted = 0;
        cout << "Enter the elements for second sets: " << endl;

        while (inserted < size2) {
            if (!(cin >> element)) {
                cout << "Error: element must be integer" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                if (!set2.contains(element)) {
                    set2.insert(element);
                    inserted++;
                } else {
                    cout << "Doublicate element, enter another value" << endl;
                }
            }
        }

        cout << "\nSet1 [" << set1.getSize() <<"]: ";
        set1.printSet();

        cout << "Set2 [" << set2.getSize() <<"]: ";
        set2.printSet();
        cout <<  endl;

        // основная часть для сравнений массивов

        while (true) {
            cout << "Select an action: " << endl;
            cout << "1. Unity sets"<< endl;
            cout << "2. Intersection sets" << endl;
            cout << "3. Difference between two sets" << endl;
            cout << "4. Change set" << endl;
            cout << "5. Exit" << endl;
            int choice;
            bool isExit = false;

            cout << "\nChoice: ";
            if (!(cin >> choice)) {
                cout << "Error: choice must be integer 1, 2, 3, 4 or 5" << endl;
            } else {
                cout << endl;
                switch(choice) {
                    case 1: {
                        Set result = Set::unionSets(set1, set2);

                        cout << "Set1 [" << size1 <<"]: ";
                        set1.printSet();
                        cout << "Set2 [" << size2 <<"]: ";
                        set2.printSet();

                        cout << "Set result [" << result.size() << "]: ";
                        result.printSet();
                        cout <<  endl;
                        break;
                    }
                    case 2: {
                        Set result = Set::intersectSets(set1, set2);

                        cout << "Set1 [" << size1 <<"]: ";
                        set1.printSet();
                        cout << "Set2 [" << size2 <<"]: ";
                        set2.printSet();

                        cout << "Set result [" << result.size() << "]: ";
                        result.printSet();
                        cout << endl;
                        break;
                    }
                    case 3: {
                        Set result = Set::differSets(set1, set2);
                        cout << "Set1 [" << size1 <<"]: ";
                        set1.printSet();

                        cout << "Set2 [" << size2 <<"]: ";
                        set2.printSet();
                        cout << "Set result [" << result.size() << "]: ";
                        result.printSet();
                        break;
                    }
                    case 4:
                        mainChangeSet(set1, set2);
                        break;
                    case 5:
                        isExit = true;
                        break;
                    default:
                        cout << "Please, select an action: 1, 2, 3, 4 or 5" << endl;
                        break;
                }
            }
            if (isExit) {
                cout << "Exit..." << endl;
                break;
            }

            
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    
    } catch (const runtime_error &e) {
        cerr << "Error: "<< e.what() << endl;
        return -1;
    } catch (const exception& m) {
        cerr << "Error: unknown error" << endl;
        return -1;
    }
    return 0;
}