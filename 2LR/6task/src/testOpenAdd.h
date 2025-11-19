#pragma once
#include <climits>

enum Status { EMPTY, OCCUPIED, DELETED };

class OpenAddHM {
private:
    struct Entry {
        int key;
        int value;
        Status state;
        Entry() : key(0), value(0), state(EMPTY) {}
    };
    Entry* table;
    int size;
    int hashFunction(int key) const {
        if (key < 0) key = -key;
        return key % size;
    }
    int linearProbe(int key, int i) const {
        return (hashFunction(key) + i) % size;
    }

public:
    OpenAddHM(int s);
    ~OpenAddHM();

    bool insert(int key, int value);
    bool erase(int key);
    int search(int key) const;
};
