#include "testOpenAdd.h"

OpenAddHM::OpenAddHM(int s) : size(s) {
    table = new Entry[size];
}

OpenAddHM::~OpenAddHM() {
    delete[] table;
}

bool OpenAddHM::insert(int key, int value) {
    for (int i = 0; i < size; i++) {
        int idx = linearProbe(key, i);
        if (table[idx].state == EMPTY || table[idx].state == DELETED) {
            table[idx].key = key;
            table[idx].value = value;
            table[idx].state = OCCUPIED;
            return true;
        } else if (table[idx].state == OCCUPIED && table[idx].key == key) {
            table[idx].value = value; 
            return true;
        }
    }
    return false;
}

bool OpenAddHM::erase(int key) {
    for (int i = 0; i < size; i++) {
        int idx = linearProbe(key, i);
        if (table[idx].state == EMPTY) return false;
        if (table[idx].state == OCCUPIED && table[idx].key == key) {
            table[idx].state = DELETED;
            return true;
        }
    }
    return false;
}
//ВОВЗРАТ VALUE 
int OpenAddHM::search(int key) const {
    for (int i = 0; i < size; i++) {
        int idx = linearProbe(key, i);
        if (table[idx].state == EMPTY) return -1;
        if (table[idx].state == OCCUPIED && table[idx].key == key) {
            return table[idx].value;
        }
    }
    return -1;
}
