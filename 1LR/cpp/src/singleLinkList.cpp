#include "singleLinkList.h"

void CreateSL(SingleLL& ls, string key) {
    if (ls.size != 0) {cout << "Error: An existing list"; return;}
    SLNode* newNode = new SLNode{key, nullptr};
    ls.head = newNode;
    ls.size++;
}

void OutputSL(SingleLL& ls) {
    cout << "Single Link List[" << ls.size << "]: ";
    SLNode* current = ls.head;
    while (current != nullptr) {
        cout << current -> key << " ";
        current = current -> next;
    }
    if (ls.size == 0) {cout << "Empty list";}
    cout << endl;
}

void FPushFront(SingleLL& ls, string key) {
    if (ls.head == nullptr) {
        CreateSL(ls, key);
        return;
    }
    SLNode* newNode = new SLNode{key, nullptr};
    newNode -> next = ls.head;
    ls.head = newNode;
    ls.size++;
}

void FPushBack(SingleLL& ls, string key) {
    if(ls.head == nullptr) {
        CreateSL(ls,key);
        return;
    }
    SLNode* current = ls.head;
    while(current -> next != nullptr) {
        current = current -> next;
    }
    SLNode* newNode = new SLNode{key,nullptr};
    current -> next = newNode;
    ls.size++;
}


void FPushAt(SingleLL& ls, int index, string value) {
    if (index < 0) {
        cout << "Ошибка: Индекс не может быть отрицательным" << endl;
        return;
    }
    
    SLNode* newNode = new SLNode{value, nullptr};
    if (index == 0) {
        newNode->next = ls.head;
        ls.head = newNode;
        cout << "Элемент '" << value << "' добавлен по индексу " << index << endl;
        return;
    }
    
    SLNode* current = ls.head;
    int currentIndex = 0;
    
    while (current != nullptr && currentIndex < index - 1) {
        current = current->next;
        currentIndex++;
    }
    
    if (current == nullptr) {
        cout << "Ошибка: Индекс " << index << " выходит за пределы списка" << endl;
        delete newNode;
        return;
    }
    
    newNode->next = current->next;
    current->next = newNode;
    
    cout << "Элемент '" << value << "' добавлен по индексу " << index << endl;
}


/*
void FPushAfter(SingleLL& ls, string target, string key) {
    if (ls.head == nullptr) {CreateSL(ls, key); return;}
    SLNode* current = ls.head;
    while(current != nullptr && current -> key != target) {
        current = current -> next;
    }
    if (current == nullptr) {
        cout << "Target didn't find" << endl;
        return;
    }

    SLNode* newNode = new SLNode{key,nullptr};
    newNode -> next = current -> next;
    current -> next = newNode;
    ls.size++;
}

void FPushBefore(SingleLL& ls, string target, string key) {
    if( ls.head == nullptr) {
        CreateSL(ls, key); 
        return;
    }
    if (ls.head -> key == target) {
        FPushFront(ls, key);
        return;
    }

    SLNode* current = ls.head;
    while (current -> next != nullptr && current->next -> key != target) {
        current = current -> next;
    }
    if (current -> next != nullptr) {
        SLNode* newNode = new SLNode{key,nullptr};
        newNode->next = current->next;
        current->next= newNode;
        ls.size++;
    } else {
        cout << "Target " << target << "didn't find" << endl;
    }
}
*/


string FGetValue(SingleLL& ls, string key) {
    if (ls.head == nullptr) {
        cout << "Error: List is not existing or empty" << endl;
        return "";
    }
    SLNode* current = ls.head;
    while (current != nullptr && current -> key != key) {
        current = current -> next;
    }
    if (current == nullptr) {
        cout << "Element " << key << " didn't find" <<endl;
        return "";
    }
    return current->key;
}

string FGetIndex(SingleLL& ls, int index) {
    if (ls.head == nullptr) {
        cout << "List is empty"<< endl;
        return "";
    }
    if (index < 0 ) {
        cout << "Incorrect index" << endl;
        return "";
    }
    SLNode* current = ls.head;
    int currentIndex = 0;

    while (current != nullptr && currentIndex < index) {
        current = current -> next;
        currentIndex++;
    }
    if (current != nullptr) {
        return current->key;
    } else {
        cout << "Element didn't find" << endl;
        return "";
    }
}

void FDelValue(SingleLL& ls, string key) {
    if (ls.head == nullptr) {
        cout << "Error: List is empty" << endl;
        return;
    }
    if (ls.head->key == key) {
        SLNode* deleteNode = ls.head;
        ls.head = ls.head->next;
        delete deleteNode;
        ls.size--;
        cout << "Элемент '" << key << "' удален" << endl;
        return;
    }
    SLNode* current = ls.head;
    while (current->next != nullptr && current->next->key != key) {
        current = current->next;
    }

    if (current->next != nullptr) {
        SLNode* deleteNode = current->next;
        current->next = current->next->next;
        delete deleteNode;
        ls.size--;
        cout << "Элемент '" << key << "' удален" << endl;  
    } else {
        cout << "Элемент '" << key << "' не найден" << endl;
    }
}

void FDelAt(SingleLL& ls, int index) {
    if (ls.head == nullptr) {
        cout << "Ошибка: List is empty" << endl;
        return;
    }
    if (index < 0 || index >= ls.size) {
        cout << "Ошибка: Incorrect index" << endl;
        return;
    }
    
    SLNode* current = ls.head;
    SLNode* prev = nullptr;
    
    // Ищем узел для удаления и предыдущий узел
    for (int indCur = 0; current != nullptr && indCur < index; indCur++) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr) {
        SLNode* temp = current;
        
        if (prev == nullptr) {
            // Удаляем первый элемент
            ls.head = current->next;
        } else {
            // Удаляем из середины или конца
            prev->next = current->next;
        }
        
        delete temp;
        ls.size--;
        cout << "Элемент с индексом " << index << " удален" << endl;
    }
}

void FDelList(SingleLL& ls) {
    if (ls.head == nullptr) {
        return;
    }
    SLNode* current = ls.head;
    SLNode* delNode;
    while (current != nullptr) {
        delNode = current;
        current = current -> next;
        delete delNode;
    }
    ls.head = nullptr;
    ls.size = 0;
}

int FGetSize(SingleLL& ls) {
    return ls.size;
}