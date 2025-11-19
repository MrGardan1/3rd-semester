#include "LRUCache.h"


//двусвязный список
DoubleLL::DoubleLL() : head(nullptr), tail(nullptr) {}

DoubleLL::~DoubleLL() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void DoubleLL::addToFront(Node* node) {
    node->prev = nullptr;
    node->next = head;
    if (head) head->prev = node;
    head = node;
    if (!tail) tail = head;
}

void DoubleLL::moveToFront(Node* node) {
    if (node == head) return;
    removeNode(node);
    addToFront(node);
}

void DoubleLL::removeNode(Node* node) {
    if (node->prev) node->prev->next = node->next;
    else head = node->next;
    if (node->next) node->next->prev = node->prev;
    else tail = node->prev;
}

Node* DoubleLL::removeLast() {
    if (!tail) return nullptr;
    Node* node = tail;
    removeNode(node);
    return node;
}

//Хэш-таблица

HashTable::HashTable(int s) : size(s) {
    table = new HashNode*[size];
    for (int i = 0; i < size; i++) table[i] = nullptr;
}

HashTable::~HashTable() {
    for (int i = 0; i < size; i++) {
        HashNode* curr = table[i];
        while (curr) {
            HashNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    delete[] table;
}

Node* HashTable::get(int key) {
    int idx = hashFunc(key);
    HashNode* cur = table[idx];
    while (cur) {
        if (cur->key == key) return cur->listNode;
        cur = cur->next;
    }
    return nullptr;
}

void HashTable::put(int key, Node* listNode) {
    int idx = hashFunc(key);
    HashNode* cur = table[idx];
    while (cur) {
        if (cur->key == key) {
            cur->listNode = listNode;
            return;
        }
        cur = cur->next;
    }
    HashNode* newNode = new HashNode(key, listNode);
    newNode->next = table[idx];
    table[idx] = newNode;
}

void HashTable::remove(int key) {
    int idx = hashFunc(key);
    HashNode* cur = table[idx];
    HashNode* prev = nullptr;
    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next;
            else table[idx] = cur->next;
            delete cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

//LRU кэш

LRUCache::LRUCache(int cap) : capacity(cap), size(0), table(cap * 2) {}

int LRUCache::get(int key) {
    Node* node = table.get(key);
    if (!node) return -1;
    list.moveToFront(node);
    return node->value;
}

void LRUCache::set(int key, int value) {
    Node* node = table.get(key);
    if (node) {
        node->value = value;
        list.moveToFront(node);
        return;
    }
    if (size == capacity) {
        Node* tail = list.removeLast();
        if (tail) {
            table.remove(tail->key);
            delete tail;
            size--;
        }
    }
    Node* newNode = new Node(key, value);
    list.addToFront(newNode);
    table.put(key, newNode);
    size++;
}