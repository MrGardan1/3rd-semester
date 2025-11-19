#include "hashTable.h"

HashTable::HashTable(size_t size) : count(size) {
    table = new Node*[count];
    for (size_t i = 0; i < count; ++i) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    clear();
    delete[] table;
}

size_t HashTable::HashFunc(int key) const {
    return static_cast<size_t>(key) % count;
}

void HashTable::clear() {
    for (size_t i = 0; i < count; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        table[i] = nullptr;
    }
}

void HashTable::SetAdd(int key) {
    size_t index = HashFunc(key);
    Node* curr = table[index];

    while (curr) {
        if (curr->key == key) return;
        curr = curr->next;
    }
    Node* newNode = new Node(key);
    newNode->next = table[index];
    if (table[index] != nullptr) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
}

void HashTable::SetDel(int key) {
    size_t index = HashFunc(key);
    Node* curr = table[index];

    while (curr) {
        if (curr->key == key) {
            if (curr->prev) curr->prev->next = curr->next;
            else table[index] = curr->next;

            if (curr->next) curr->next->prev = curr->prev;

            delete curr;
            return;
        }
        curr = curr->next;
    }
    std::cout << "Error: element didn't find\n";
}

bool HashTable::Set_At(int key) const {
    size_t index = HashFunc(key);
    Node* curr = table[index];

    while (curr) {
        if (curr->key == key) return true;
        curr = curr->next;
    }
    return false;
}

bool HashTable::LoadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;
    clear();

    int key;
    while (fin >> key) {
        SetAdd(key);
    }
    return true;
}

bool HashTable::SaveToFile(const std::string& filename) const {
    std::ofstream fout(filename);
    if (!fout.is_open()) return false;

    for (size_t i = 0; i < count; ++i) {
        Node* curr = table[i];
        while (curr) {
            fout << curr->key << " ";
            curr = curr->next;
        }
    }
    return true;
}
