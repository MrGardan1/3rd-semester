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
        ls.size++;
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
    ls.size++;
    cout << "Элемент '" << value << "' добавлен по индексу " << index << endl;
}


void FPushAfter(SingleLL& ls, string target, string key) {
    if (ls.head == nullptr) {
        cout << "Ошибка: Список пуст" << endl;
        return;
    }
    
    SLNode* current = ls.head;
    while(current != nullptr && current -> key != target) {
        current = current -> next;
    }
    
    if (current == nullptr) {
        cout << "Элемент '" << target << "' не найден" << endl;
        return;
    }

    SLNode* newNode = new SLNode{key, nullptr};
    newNode -> next = current -> next;
    current -> next = newNode;
    ls.size++;
    cout << "Элемент '" << key << "' добавлен после '" << target << "'" << endl;
}

void FPushBefore(SingleLL& ls, string target, string key) {
    if(ls.head == nullptr) {
        cout << "Ошибка: Список пуст" << endl;
        return;
    }
    
    if (ls.head -> key == target) {
        FPushFront(ls, key);
        cout << "Элемент '" << key << "' добавлен перед '" << target << "'" << endl;
        return;
    }

    SLNode* current = ls.head;
    while (current -> next != nullptr && current->next -> key != target) {
        current = current -> next;
    }
    
    if (current -> next != nullptr) {
        SLNode* newNode = new SLNode{key, nullptr};
        newNode->next = current->next;
        current->next = newNode;
        ls.size++;
        cout << "Элемент '" << key << "' добавлен перед '" << target << "'" << endl;
    } else {
        cout << "Элемент '" << target << "' не найден" << endl;
    }
}


void FGetValue(SingleLL& ls, string key) {
    if (ls.head == nullptr) {
        cout << "Error: List is not existing or empty" << endl;
        return;
    }
    SLNode* current = ls.head;
    int index;
    while (current != nullptr && current -> key != key) {
        current = current -> next;
        index++;
    }
    if (current == nullptr) {
        cout << "Element " << key << " didn't find" <<endl;
        return;
    }
    cout << "SingleList[" << index <<"] = " << key << endl;
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

void FDelFront(SingleLL& ls) {
    if (ls.head == nullptr) {
        cout << "Ошибка: пустой список" << endl;
        return;
    }
    SLNode* delNode = ls.head;
    ls.head = ls.head->next;
    delete delNode;
    ls.size--;
}

void FDelBack(SingleLL& ls) {
    if (ls.head == nullptr) {
        cout << "Ошибка: пустой список" << endl;
    }

    if (ls.head ->next == nullptr) {
        delete ls.head;
        ls.head = nullptr;
        ls.size = 0;
        cout << "Последний элемент удален" << endl;
        return;
    }

    SLNode* current = ls.head;
    while (current -> next -> next != nullptr) {
        current = current ->next;
    }
    delete current -> next;
    current->next = nullptr;
    ls.size--;

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

void FDelAfter(SingleLL& ls, string target) {
    if (ls.head == nullptr) {
        cout << "Ошибка: пустой список" << endl;
        return;
    }

    SLNode* current = ls.head;
    while (current != nullptr && current->key != target) {
        current = current->next;
    }
    if (current == nullptr) {
        cout << "Ошибка: элемент '" << target << "' не найден" << endl;
        return;
    }
    
    if (current->next == nullptr) {
        cout << "Ошибка: после элемента '" << target << "' нет элементов" << endl;
        return;
    }

    SLNode* delNode = current->next;
    current->next = current->next->next;
    delete delNode;
    ls.size--;
    cout << "Элемент после " << target << " удален" << endl;
}

void FDelBefore(SingleLL& ls, string target) {
    if (ls.head == nullptr) {
        cout << "Ошибка: пустой лист" << endl;
        return;
    }
    //если первый элемент
    if (ls.head->key == target) {
        cout << "Перед первым элементом ничего нет" << endl;
        return;
    }

    //удаление первый если искомый второй
    if (ls.head->next != nullptr && ls.head->next->key == target) {
        FDelFront(ls);
        cout << "Элемент перед " << target << " удален" << endl;
        return;
    }

    SLNode* prev = nullptr;
    SLNode* current = ls.head;
    while (current->next != nullptr && current->next->key != target) {
        prev = current;
        current = current->next;
    }

    if (current->next == nullptr) {
        cout << "Элемент " << target << " не найден" << endl;
        return;
    }

    if (prev != nullptr) {
        SLNode* delNode = prev->next;
        prev->next = current->next;
        delete delNode;
        ls.size--;
        cout << "Элемент перед " << target << " удален" << endl;
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