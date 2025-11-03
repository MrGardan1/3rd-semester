#include "doubleLinkList.h"

void CreateDL(DoubleList& ls) {
    ls.head = nullptr;
    ls.tail = nullptr;
    ls.size = 0;
}

void OutputDL(DoubleList& ls) {
    cout << "DoubleList [" << ls.size << "]: ";
    
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }
    DLNode* current = ls.head;
    for (int i = 0; i < ls.size; i++) {
        cout << current -> key << " ";
        current = current -> next;
    }
    cout << endl;
}

void RevOutputDL(DoubleList& ls) {
    cout << "DoubleList [" << ls.size << "] reverse: ";

    if (ls.tail == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* current = ls.tail;
    while (current != nullptr) {
        cout << current->key << " ";
        current = current->past;
    }
    cout << endl;
}

void DLFind(DoubleList& ls, string key) {
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* current = ls.head;
    for (int i = 0; i < ls.size; i++) {
        if ( current -> key == key) {
            cout << "Element [" << i <<"] = " << current -> key << endl;
            return;
        }
        current = current -> next;
    }
    cout << "Element didn't find" << endl;
}

void DLPushFront(DoubleList& ls, string key) {
    DLNode* newNode = new DLNode{key, nullptr, nullptr};
    
    if (ls.head == nullptr) {
        ls.head = newNode;
        ls.tail = newNode;
    } else {
        newNode->next = ls.head;
        ls.head->past = newNode;
        ls.head = newNode;
    }
    ls.size++;
    cout << "Элемент " << key << " добавлен в начало " << endl;
}

void DLPushBack(DoubleList& ls, string key) {
    DLNode* newNode = new DLNode{key, nullptr, nullptr};
    
    if (ls.tail == nullptr) {
        ls.head = newNode;
        ls.tail = newNode;
    } else {
        ls.tail->next = newNode;
        newNode->past = ls.tail;
        ls.tail = newNode;
    }
    ls.size++;
    cout << "Элемент " << key << " добавлен в конец" << endl;
}

void DLPushAt(DoubleList& ls, int index, string key) {
    if (index < 0 || index > ls.size) {
        cout << "Error: Incorrect index" << endl;
        return;
    }
    if (index == 0) {
        DLPushFront(ls, key);
        return;
    }

    if (index == ls.size) {
        DLPushBack(ls, key);
        return;  
    }

    DLNode* newNode = new DLNode{key, nullptr, nullptr};
    DLNode* current = ls.head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }

    if (current != nullptr) {
        newNode->next = current;
        newNode->past = current->past;
        
        if (current->past != nullptr) {
            current->past->next = newNode;
        }
        current->past = newNode;
        
        ls.size++;
        cout << "Элемент '" << key << "' добавлен по индексу " << index << endl;
    }
}

void DLPushAft(DoubleList& ls, string target, string key) {
    if (ls.head == nullptr) {
        cout << "Ошибка: Список пуст" << endl;
        return;
    }
    DLNode* current = ls.head;
    while (current != nullptr && current->key != target) {
        current = current->next;
    }
    if (current == nullptr) {
        cout << "Элемент '" << target << "' не найден" << endl;
        return;
    }
    
    DLNode* newNode = new DLNode{key, nullptr, nullptr};
    newNode->past = current;
    newNode->next = current->next;
    
    if (current->next != nullptr) {
        current->next->past = newNode;
    } else {
        ls.tail = newNode;
    }
    current->next = newNode;
    ls.size++;

    cout << "Элемент " << key << " добавлен после " << target << endl;
}

void DLPushBef(DoubleList& ls, string target, string key) {
    if (ls.head == nullptr) {
        cout << "Ошибка: Список пуст" << endl;
        return;
    }
    DLNode* current = ls.head;
    while (current != nullptr && current->key != target) {
        current = current->next;
    }
    if (current == nullptr) {
        cout << "Элемент '" << target << "' не найден" << endl;
        return;
    }

    DLNode* newNode = new DLNode{key, nullptr, nullptr};
    newNode->past = current->past;
    newNode->next = current;
    if (current->past != nullptr) {
        current->past->next = newNode;
    } else {
        ls.head = newNode;
    }
    current->past = newNode;
    ls.size++;
    cout << "Элемент " << key << " добавлен перед " << target << endl;
}

void DLDelValue(DoubleList& ls, string key) {
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }
    
    DLNode* current = ls.head;
    while (current != nullptr && current->key != key) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Element didn't find" << endl;
        return;
    }
    
    if (current == ls.head) {
        ls.head = current->next;
    }
    if (current == ls.tail) {
        ls.tail = current->past;
    }
    
    if (current->past != nullptr) {
        current->past->next = current->next;
    }
    if (current->next != nullptr) {
        current->next->past = current->past;
    }
    
    delete current;
    ls.size--;
    cout << "Элемент " << key << " удален" << endl;
}

void DLDelAt(DoubleList& ls, int index) {
    if (ls.head == nullptr) {
        cout << "Error: List is empty" << endl;
        return;
    }

    if (index < 0 || index >= ls.size) {
        cout << "Error: incorrect index" << endl;
        return;
    }

    if (index == 0) {
        DLDelFront(ls);
        return;
    }
    if (index == ls.size - 1) {
        DLDelBack(ls);
        return;
    }

    DLNode* current = ls.head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }

    if (current != nullptr) {
        if (current->past != nullptr) {
            current->past->next = current->next;
        }
        if (current->next != nullptr) {
            current->next->past = current->past;
        }
        delete current;
        ls.size--;
        cout << "Элемент с индексом " << index << " удален" << endl;
    }
}

void DLDelFront(DoubleList& ls) {
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* delNode = ls.head;
    ls.head = ls.head->next;
    
    if (ls.head != nullptr) {
        ls.head->past = nullptr;
    } else {
        // Если список стал пустым, обновляем хвост
        ls.tail = nullptr;
    }
    delete delNode;
    ls.size--;
    cout << "Первый элемент удален" << endl;
}

void DLDelBack(DoubleList& ls) {
    if (ls.tail == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* delNode = ls.tail;
    ls.tail = ls.tail->past;
    
    if (ls.tail != nullptr) {
        ls.tail->next = nullptr;
    } else {
        ls.head = nullptr;
    }
    delete delNode;
    ls.size--;
    cout << "Последний элемент удален" << endl;
}

void DLDelAfter(DoubleList& ls, string target) {
    if (ls.head == nullptr) {
        cout << "Ошибка: пустой список" << endl;
        return;
    }
    DLNode* current = ls.head;
    while (current != nullptr && current->key != target) {
        current = current->next;
    }
    if (current == nullptr) {
        cout << "Элемент " << target << " не найден" << endl;
        return;
    }
    if (current->next == nullptr) {
        cout << "После элемента " << target << " нет элементов" << endl;
        return;
    }
    DLNode* delNode = current->next;
    current->next = delNode->next;
    
    if (delNode->next != nullptr) {
        delNode->next->past = current;
    } else {
        ls.tail = current;
    }
    
    delete delNode;
    ls.size--;
    cout << "Элемент после " << target << " удален" << endl;
}

void DLDelBefore(DoubleList& ls, string target) {
    if (ls.head == nullptr) {
        cout << "Ошибка: пустой список" << endl;
        return;
    }

    DLNode* current = ls.head;
    while (current != nullptr && current->key != target) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Элемент '" << target << "' не найден" << endl;
        return;
    }
    if (current->past == nullptr) {
        cout << "Перед элементом '" << target << "' нет элементов" << endl;
        return;
    }

    DLNode* delNode = current->past;
    current->past = delNode->past;
    if (delNode->past != nullptr) {
        delNode->past->next = current;
    } else {
        ls.head = current;
    }
    
    delete delNode;
    ls.size--;
    cout << "Элемент перед '" << target << "' удален" << endl;
}

void DLDelAll(DoubleList& ls) {
    if (ls.head == nullptr) return;
    
    DLNode* current = ls.head;
    while (current != nullptr) {
        DLNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    
    ls.head = nullptr;
    ls.tail = nullptr;
    ls.size = 0;
    cout << "Все элементы удалены" << endl;
}

int DLGetSize(DoubleList& ls) {
    return ls.size;
}

string DLGetAt(DoubleList& ls, int index) {
    if (index < 0 || index >= ls.size )  {
        cout << "Out of range" << endl;
        return "";
    }
    DLNode* current = ls.head;
    for (int i = 0; current != nullptr && i < index; i++) {
        current = current -> next;
    }

    if (current != nullptr ) {
        return current->key;
    } else {
        cout << index <<" didn't find" << endl;
        return "";
    }
}

string DLGetFront(DoubleList& ls) {
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return "";
    }
    return ls.head->key;
}

string DLGetBack(DoubleList& ls) {
    if (ls.tail == nullptr) {
        cout << "List is empty" << endl;
        return "";
    }
    return ls.tail->key;
}