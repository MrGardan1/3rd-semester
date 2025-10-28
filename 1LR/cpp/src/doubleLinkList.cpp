#include "doubleLinkList.h"

void CreateDL(DoubleList& ls) {
    ls.head = nullptr;
    ls.size = 0;
}

void OutputDL(DoubleList& ls) {
    cout << "DoubleList [" << ls.size << "]: ";
    
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }
    DLNode* current = ls.head;
    for (int i = 0; i< ls.size; i++) {
        cout << current -> key << " ";
        current = current -> next;
    }
    cout << endl;
}

void RevOutputDL(DoubleList& ls) {
    cout << "DoubleList [" << ls.size << "] reverse: ";

    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* current = ls.head;
    while (current->next != nullptr) {
        current = current->next;
    }
    
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
    newNode -> next = ls.head;

    if (ls.head != nullptr) {
        ls.head->past = newNode;
    }
    ls.head = newNode;
    ls.size++;
    cout << "Элемент " << key << " добавлен в начало " << endl;
}

void DLPushBack(DoubleList& ls, string key) {
    DLNode* newNode = new DLNode{key, nullptr, nullptr};
    
    if (ls.head == nullptr) {
        ls.head = newNode;
    } else {
        DLNode* tail = ls.head;
        while (tail->next != nullptr) {
            tail = tail->next;
        }
        tail->next = newNode;
        newNode->past = tail;
        cout << "Элемент " << key << " добавлен в конец" << endl;

    }
    ls.size++;
}

void DLPushAt(DoubleList& ls, int index, string key) {
    if (index < 0 || index > ls.size) {
        cout << "Error: Incorrect index" << endl;
        return;
    }

    DLNode* newNode = new DLNode{key, nullptr, nullptr};

    if (ls.head == nullptr) {
        if (index == 0) {
            DLPushFront(ls, key);
        } else {
            cout << "Error: List is empty, can only insert at index 0" << endl;
            delete newNode;
        }
        return;
    }

    if (index == 0) {
        DLPushFront(ls, key);
        return;
    }

    if (index == ls.size) {
        DLPushBack(ls, key);
    }

    DLNode* current = ls.head;
    for (int i = 0; current != nullptr && i < index; i++) {
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
        cout << "Элемент " << key << " добавлен по индексу " << index << endl;
    }
}
/*
void DLPushAft(DoubleList& ls,string target, string key) {
        DLNode* newNode = new DLNode {key,nullptr, nullptr};
        if (ls.head == nullptr) {
            ls.head = newNode;
            ls.size++;
            return;
        }

        DLNode* current = ls.head;
        while (current != nullptr && current -> key != target) {
            current = current -> next;
        }

        if (current != nullptr) {

            newNode -> past = current;
            newNode -> next = current -> next;

            if (current -> next != nullptr) {
                current -> next -> past = newNode;
            }
            current -> next = newNode;
            ls.size++;
        } else {
            cout << "Element " << target << " didn't find" << endl;
            delete newNode;
            return;
        }

    }

void DLPushBef(DoubleList& ls, string target, string key) {
    DLNode* newNode = new DLNode{key,nullptr, nullptr};
    if (ls.head == nullptr) {
        ls.head = newNode;
        ls.size++;
        return;
    }

    DLNode* current = ls.head;
    while (current != nullptr && current -> key != target ) {
        current = current -> next;
    }

    if (current != nullptr) {
        newNode -> past = current ->past;
        newNode -> next = current;

        if (current -> past != nullptr) {
            current -> past -> next = newNode;
        } else {
            ls.head = newNode;
        }
        current -> past = newNode;
        ls.size++;
    } else {
        cout << "Element " << target << " didn't find" << endl;
        delete newNode;
    }
}
*/
void DLDelValue(DoubleList& ls, string key) {
    
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }
    if (ls.head->key == key) {
        DLNode* temp = ls.head;
        ls.head = ls.head -> next;
        delete temp;
        ls.size--;
        return;
    }

    DLNode* current = ls.head;
    while (current != nullptr && current -> key != key) {
        current = current -> next;
    }

    if (current != nullptr) {
        if (current -> past != nullptr) {
            (current -> past) -> next = current->next;
        }
        if (current -> next != nullptr) {
            current -> next ->past = current -> past;
        }
        delete current;
        ls.size--;
    } else {
        cout << "Element didn't find" << endl;
    }
}

void DLDelAt(DoubleList& ls, int index) {
    if (ls.head == nullptr) {
        cout <<"Error: List is empty" << endl;
        return;
    }

    if (index < 0|| index > ls.size) {
        cout << "Error: incorrect index" << endl;
        return;
    }

    if (index == 0) {
        DLDelFront(ls);
    }

    DLNode* current = ls.head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current -> next;
    }

    if (current != nullptr) {
        if (current -> past != nullptr) {
            (current -> past) -> next = current->next;
        }
        if (current -> next != nullptr) {
            current -> next ->past = current -> past;
        }
        delete current;
        ls.size--;
    }
}

void DLDelFront(DoubleList& ls) {
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* delNode = ls.head;
    if (ls.head ->next != nullptr) {
        ls.head = ls.head->next;
        ls.head ->past = nullptr;
    } else {
        ls.head = nullptr;
    }
    delete delNode;
    ls.size--;
}

void DLDelBack(DoubleList& ls) {
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return;
    }

    DLNode* current = ls.head;
    while(current -> next != nullptr) {
        current = current -> next;
    }
    
    if (current -> past != nullptr) {
        current -> past ->next = nullptr;
    } else {
        ls.head = nullptr;
    }
    delete current;
    ls.size--;
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
    ls.size = 0;
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
    if (ls.head == nullptr) {
        cout << "List is empty" << endl;
        return "";
    }

    DLNode* current = ls.head;
    while (current -> next != nullptr) {
        current = current -> next;
    }
    return current -> key;
}