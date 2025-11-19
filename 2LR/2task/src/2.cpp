#include "hashTable.h"
#include <iostream>
#include <stdexcept>
using namespace std;

enum class COMMAND {
    SETADD,
    SETDEL,
    SET_AT
};

// Дополнительные операции Множества 
// Реализовать следующие операции со множеством: объединение множеств, 
// пересечение множеств, разность множеств.

int main(int argc, char* argv[] ) {

    if(argc !=6) {
        cout << "\nError: Invalid query" << endl;
        cout << "\nUse: ./program --file filePath -query COMMAND key" << "    for SETADD, SETDEL" << endl;        
        cout << "Available Commands: SETADD(int key), SETDEL(int key), SET_AT(int key)\n" << endl;
        return -1;
    }

    string filePath;
    string command;
    COMMAND cmd;
    int key;
    try {
        string arg = argv[1];
        if (arg == "--file") {
            filePath = argv[2];
        } else {
            cout << "Error: invalid argument" << endl;
            cout << "Use: --file filePath" << endl;
            return -1;
        }

        arg = argv[3];
        try{
            key = stoi(argv[5]);
        } catch (const exception& e) {
            throw invalid_argument("key must be integer");
        }
        if (arg == "-query") {
            command = string(argv[4]);

            if (command == "SETADD") {
                cmd = COMMAND::SETADD;
            } else if (command == "SETDEL") {
                cmd = COMMAND::SETDEL;
            } else if (command == "SET_AT") {
                cmd  = COMMAND::SET_AT;
            } else {
                cout << "Error: invalid command" << endl;
                return -1;
            }
        }
    } catch (const invalid_argument &e) {
        cout << "Error: Invalid argument, " << e.what() << endl;
        return -1;
    }
    cout << "FilePath: " << filePath << endl;
    cout << "Query: "; //вывод аргументов
    cout <<"command: " << command << ", key: " << key << endl;
    cout << endl;

    HashTable data;

    if (!data.LoadFromFile(filePath)) {
        cout << "File didn't find or empty. Create new file in: " << filePath << endl;
    }

    switch(cmd) {
        case COMMAND::SETADD:
            data.SetAdd(key);
            data.SaveToFile(filePath);
            break;
        case COMMAND::SETDEL:
            data.SetDel(key);
            data.SaveToFile(filePath);
            break;
        case COMMAND::SET_AT: {
            bool success = data.Set_At(key);
            cout << key << (success ? " Exist" : " Not exits") << endl;
            break;
        }
        default:
            cout << "Error: unknown command" << endl;
    }
    
    return 0;
}