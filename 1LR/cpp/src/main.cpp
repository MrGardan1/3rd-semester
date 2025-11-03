#include "main.h"

Array* currentArray = nullptr;
SingleLL* currentList = nullptr;
DoubleList* currentDList = nullptr;
Stack* currentStack = nullptr;
Queue* currentQueue = nullptr;
Tree* currentTree = nullptr;

int main() {

    string command;

    cout << "=== Интерфейс ===" << endl;
    cout << "Введите команды (HELP MAIN для справки, EXIT для выхода): " << endl;
    while (true) {
        cout << "> ";
        getline(cin, command);
        if (command.empty()) continue;

        mainCommand(command);
    }

    return 0;
}

void mainCommand(const string& command) {
    stringstream ss(command);
    string cmd;
    ss >> cmd;
    
    //==================================МАССИВ==========================================================
    if (cmd == "MCREATE") {
        if (currentArray) {
            if (currentArray->data) delete[] currentArray->data;
            delete currentArray;
        }
        currentArray = new Array;
        MCreate(*currentArray);
        cout << "Массив создан" << endl;
    }
    else if (cmd == "MPUSH") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        string type;
        string value;
        ss >> type;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        
        if (type == "F") {
            MInsert(*currentArray, 0, value);
        }
        else if (type == "B") {
            MPushBack(*currentArray, value);
        }
        else {
            int index = stoi(type);
            MInsert(*currentArray, index, value);
        }
        cout << "Элемент добавлен" << endl;
    }
    else if (cmd == "MDEL") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        
        if (type == "F") {
            MDelIndex(*currentArray, 0);
        }
        else if (type == "B") {
            MDelIndex(*currentArray, currentArray->size - 1);
        }
        else if (type == "V") {
            string value;
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            for (int i = 0; i < currentArray->size; i++) {
                if (currentArray->data[i] == value) {
                    MDelIndex(*currentArray, i);
                    break;
                }
            }
        }
        else {
            int index = stoi(type);
            MDelIndex(*currentArray, index);
        }
        cout << "Элемент удален" << endl;
    }
    else if (cmd == "MGET") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        
        if (type == "F") {
            if (currentArray->size > 0)
                cout << "Первый элемент: " << MGetValue(*currentArray, 0) << endl;
        }
        else if (type == "B") {
            if (currentArray->size > 0)
                cout << "Последний элемент: " << MGetValue(*currentArray, currentArray->size - 1) << endl;
        }
        else {
            int index = stoi(type);
            if (index >= 0 && index < currentArray->size)
                cout << "Элемент по индексу " << index << ": " << MGetValue(*currentArray, index) << endl;
            else
                cout << "Неверный индекс" << endl;
        }
    }
    else if (cmd == "MFIND") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        string value;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        bool found = false;
        for (int i = 0; i < currentArray->size; i++) {
            if (currentArray->data[i] == value) {
                cout << "Значение найдено по индексу: " << i << endl;
                found = true;
                break;
            }
        }
        if (!found) cout << "Значение не найдено" << endl;
    }
    else if (cmd == "MSIZE") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        cout << "Размер массива: " << MGetSize(*currentArray) << endl;
    }
    else if (cmd == "MPRINT") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        OutputM(*currentArray);
    }
    else if (cmd == "MSAVE") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        string filename;
        ss >> filename;
        saveArrayToFile(*currentArray, filename);
    }
    else if (cmd == "MLOAD") {
        string filename;
        ss >> filename;
        if (!currentArray) {
            currentArray = new Array;
        }
        loadArrayFromFile(*currentArray, filename);
    }
    else if (cmd == "MREPLACE") {
        if (!currentArray) {
            cout << "Ошибка: Массив не создан" << endl;
            return;
        }
        int index;
        string value;
        ss >> index;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        if (index >= 0 && index < currentArray->size) {
            currentArray->data[index] = value;
            cout << "Элемент заменен" << endl;
        } else {
            cout << "Неверный индекс" << endl;
        }
    }

    // ===========================ОДНОСВЯЗНЫЙ СПИСОК================================================
    else if (cmd == "FCREATE") {
        if (currentList) {
            FDelList(*currentList);
            delete currentList;
        }
        currentList = new SingleLL{nullptr, 0};
        currentList->size = 0;
        cout << "Односвязный список создан" << endl;
    }
    else if (cmd == "FPUSH") {
    if (!currentList) {
        cout << "Ошибка: Список не создан" << endl;
        return;
    }
    string type;
    string value;
    ss >> type;
    
    
    if (type == "F") {
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        FPushFront(*currentList, value);
        cout << "Элемент добавлен в начало" << endl;
    } else if (type == "B") {
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        FPushBack(*currentList, value);
        cout << "Элемент добавлен в конец" << endl;
    } else if (type == "AFT") {
        string target;
        ss >> target;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        FPushAfter(*currentList, target, value);   
    } else if (type == "BEF") {
        string target;
        ss >> target;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        FPushBefore(*currentList, target, value);
    } else {
        int index = stoi(type);
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        FPushAt(*currentList, index, value);
    }
} 
    else if (cmd == "FDEL") {
        if (!currentList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        
        if (type == "F") {
            FDelFront(*currentList);
        }
        else if (type == "B") {
            FDelBack(*currentList);
        }
        else if (type == "V") {
            string value;
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            FDelValue(*currentList, value);
        } else if (type == "AFT") {
            string target;
            getline(ss,target);
            target = target.empty() ? "" : target.substr(1);
            FDelAfter(*currentList, target);
        } else if (type == "BEF") {
            string target;
            getline(ss,target);
            target = target.empty() ? "" : target.substr(1);
            FDelBefore(*currentList, target);
        } else {
            int index = stoi(type);
            FDelAt(*currentList, index);
        }
    }
    else if (cmd == "FGET") {
        if (!currentList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        
        if (type == "F") {
            if (currentList->head)
                cout << "Первый элемент: " << currentList->head->key << endl;
        }
        else if (type == "B") {
            if (currentList->head) {
                SLNode* current = currentList->head;
                while (current->next) current = current->next;
                cout << "Последний элемент: " << current->key << endl;
            }
        }
        else {
            int index = stoi(type);
            string value = FGetIndex(*currentList, index);
            if (!value.empty())
                cout << "Элемент по индексу " << index << ": " << value << endl;
        }
    }
    else if (cmd == "FFIND") {
        if (!currentList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string value;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        FGetValue(*currentList, value);
    }
    else if (cmd == "FPRINT") {
        if (!currentList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        OutputSL(*currentList);
    }
    else if (cmd == "FSAVE") {
        if (!currentList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string filename;
        ss >> filename;
        saveListToFile(*currentList, filename);
    }
    else if (cmd == "FLOAD") {
        string filename;
        ss >> filename;
        if (!currentList) {
            currentList = new SingleLL{nullptr, 0};
        }
        loadListFromFile(currentList, filename);
    }

    // ========================================ДВУСВЯЗНЫЙ СПИСОК====================================================
    else if (cmd == "LCREATE") {
        if (currentDList) {
            DLDelAll(*currentDList);
            delete currentDList;
        }
        currentDList = new DoubleList;
        CreateDL(*currentDList);
        cout << "Двусвязный список создан" << endl;
    }
    else if (cmd == "LPUSH") {
        if (!currentDList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string type;
        string value;
        ss >> type;
        if (type == "F") {
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            DLPushFront(*currentDList, value);
        }
        else if (type == "B") {
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            DLPushBack(*currentDList, value);
        }
        else if (type == "AFT") {
            string target;
            ss >> target;
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            DLPushAft(*currentDList, target, value);
        } else if (type == "BEF") {
            string target;
            ss >> target;
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            DLPushBef(*currentDList, target, value);
        } else {
            int index = stoi(type);
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            DLPushAt(*currentDList, index,value);
        }
    }
    else if (cmd == "LDEL") {
        if (!currentDList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        
        if (type == "F") {
            DLDelFront(*currentDList);
        }
        else if (type == "B") {
            DLDelBack(*currentDList);
        }
        else if (type == "V") {
            string value;
            getline(ss, value);
            value = value.empty() ? "" : value.substr(1);
            DLDelValue(*currentDList, value);
        } else if (type == "AFT") {
            string target;
            getline(ss, target);
            target = target.empty() ? "" : target.substr(1);
            DLDelAfter(*currentDList, target);
        } else if (type == "BEF") {
            string target;
            getline(ss, target);
            target = target.empty() ? "" : target.substr(1);
            DLDelBefore(*currentDList, target);
        } else {
            int index = stoi(type);
            DLDelAt(*currentDList, index);
        }
    }
    else if (cmd == "LGET") {
        if (!currentDList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        
        if (type == "F") {
            cout << "Первый элемент: " << DLGetFront(*currentDList) << endl;
        }
        else if (type == "B") {
            cout << "Последний элемент: " << DLGetBack(*currentDList) << endl;
        }
        else {
            int index = stoi(type);
            string value = DLGetAt(*currentDList, index);
            if (!value.empty())
                cout << "Элемент по индексу " << index << ": " << value << endl;
        }
    }
    else if (cmd == "LPRINT") {
        if (!currentDList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string type;
        ss >> type;
        if (type == "B") 
            RevOutputDL(*currentDList);
        else 
            OutputDL(*currentDList);
    }
    else if (cmd == "LSAVE") {
        if (!currentDList) {
            cout << "Ошибка: Список не создан" << endl;
            return;
        }
        string filename;
        ss >> filename;
        saveDoubleListToFile(currentDList, filename);
    }
    else if (cmd == "LLOAD") {
        string filename;
        ss >> filename;
        loadDoubleListFromFile(currentDList, filename);
    }

    // ======================================= СТЕК ====================================================================
    else if (cmd == "SCREATE") {
        if (currentStack) {
            SDelAll(currentStack);
            delete currentStack;
        }
        currentStack = new Stack;
        CreateS(*currentStack);
        cout << "Стек создан" << endl;
    }
    else if (cmd == "SPUSH") {
        if (!currentStack) {
            cout << "Ошибка: Стек не создан" << endl;
            return;
        }
        string value;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        SPush(currentStack, value);
    }
    else if (cmd == "SPOP") {
        if (!currentStack) {
            cout << "Ошибка: Стек не создан" << endl;
            return;
        }
        string value = SPop(currentStack);
        if (!value.empty()) cout << "Удален элемент: " << value << endl;
    }
    else if (cmd == "STOP") {
        if (!currentStack) {
            cout << "Ошибка: Стек не создан" << endl;
            return;
        }
        if (currentStack->size > 0)
            cout << "Верхний элемент: " << currentStack->data[currentStack->size - 1] << endl;
    }
    else if (cmd == "SPRINT") {
        if (!currentStack) {
            cout << "Ошибка: Стек не создан" << endl;
            return;
        }
        OutputS(*currentStack);
    }
    else if (cmd == "SSAVE") {
        if (!currentStack) {
            cout << "Ошибка: Стек не создан" << endl;
            return;
        }
        string filename;
        ss >> filename;
        saveStackToFile(currentStack, filename);
    }
    else if (cmd == "SLOAD") {
        string filename;
        ss >> filename;
        loadStackFromFile(currentStack, filename);
    }

    // ====================================== ОЧЕРЕДЬ ==========================================================
    else if (cmd == "QCREATE") {
        if (currentQueue) {
            QDelAll(currentQueue);
            delete currentQueue;
        }
        currentQueue = new Queue;
        CreateQ(*currentQueue);
        cout << "Очередь создана" << endl;
    }
    else if (cmd == "QPUSH") {
        if (!currentQueue) {
            cout << "Ошибка: Очередь не создана" << endl;
            return;
        }
        string value;
        getline(ss, value);
        value = value.empty() ? "" : value.substr(1);
        QPush(*currentQueue, value);
    }
    else if (cmd == "QPOP") {
        if (!currentQueue) {
            cout << "Ошибка: Очередь не создана" << endl;
            return;
        }
        QPop(*currentQueue);
        cout << "Элемент удален из очереди" << endl;
    }
    else if (cmd == "QINDEX") {
        if (!currentQueue) {
            cout << "Ошибка: Очередь не создана" << endl;
            return;
        }
        string type;
        ss >> type;
        int index = stoi(type);
        cout << "Queue [" << index<<"]: " << QGetIndex(*currentQueue, index) << endl;
    }
    else if (cmd == "QPRINT") {
        if (!currentQueue) {
            cout << "Ошибка: Очередь не создана" << endl;
            return;
        }
        OutputQ(*currentQueue);
    }
    else if (cmd == "QSAVE") {
        if (!currentQueue) {
            cout << "Ошибка: Очередь не создана" << endl;
            return;
        }
        string filename;
        ss >> filename;
        saveQueueToFile(currentQueue, filename);
    }
    else if (cmd == "QLOAD") {
        string filename;
        ss >> filename;
        loadQueueFromFile(currentQueue, filename);
    }

    // ======================================= ДЕРЕВО ========================================
    else if (cmd == "TCREATE") {
        if (currentTree) {
            deleteTree(currentTree);
            delete currentTree;
        }
        currentTree = new Tree;
        initTree(currentTree);
        cout << "AVL дерево создано" << endl;
    }
    else if (cmd == "TINSERT") {
        if (!currentTree) {
            cout << "Ошибка: Дерево не создано" << endl;
            return;
        }
        int key;
        if (ss >> key) {
            insert(currentTree, key);
            cout << "Элемент " << key << " добавлен в дерево" << endl;
        } else {
            cout << "Ошибка: Неверный формат числа" << endl;
        }
    }
    else if (cmd == "TREMOVE") {
        if (!currentTree) {
            cout << "Ошибка: Дерево не создано" << endl;
            return;
        }
        int key;
        if (ss >> key) {
            if (remove(currentTree, key)) {
                cout << "Элемент " << key << " удален из дерева" << endl;
            } else {
                cout << "Элемент " << key << " не найден" << endl;
            }
        } else {
            cout << "Ошибка: Неверный формат числа" << endl;
        }
    }
    else if (cmd == "TFIND") {
        if (!currentTree) {
            cout << "Ошибка: Дерево не создано" << endl;
            return;
        }
        int key;
        if (ss >> key) {
            if (search(currentTree, key)) {
                cout << "Элемент " << key << " найден в дереве" << endl;
            } else {
                cout << "Элемент " << key << " не найден" << endl;
            }
        } else {
            cout << "Ошибка: Неверный формат числа" << endl;
        }
    }
    else if (cmd == "TPRINT") {//ДОДЕЛАТЬ ПРИНТ 4 СПОСОБА ИНФА В ТЕТРАДИ
        if (!currentTree) {
            cout << "Ошибка: Дерево не создано" << endl;
            return;
        }

        // Все 4 вида обходов
        printPreOrder(currentTree);// Прямой корень - лев - прав
        printInOrder(currentTree);// Центрированный лев - корень - прав
        printPostOrder(currentTree);// Обратный 
        printLevelOrder(currentTree);// В ширину (BFS)
    }
    else if (cmd == "TSAVE") {
        if (!currentTree) {
            cout << "Ошибка: Дерево не создано" << endl;
            return;
        }
        string filename;
        if (ss >> filename) {
            saveTreeToFile(currentTree, filename);
        } else {
            cout << "Ошибка: Не указано имя файла" << endl;
        }
    }
    else if (cmd == "TLOAD") {
        string filename;
        if (ss >> filename) {
            loadTreeFromFile(currentTree, filename);
        } else {
            cout << "Ошибка: Не указано имя файла" << endl;
        }
    }
// ======================================  HELP =================================================
    else if (cmd == "HELP") {
        string type;
        ss >> type;
        if (type == "MAIN") {
            printMain();
        } else if (type == "ALL") {
            printAll();
        } else if (type == "M") {
            printM();
        } else if (type == "F") {
            printF();
        } else if (type == "L") {
            printL();
        } else if (type == "Q") {
            printQ();
        } else if (type == "S") {
            printS();
        } else if (type == "T") {
            printT();
        } else {
            cout << "Используйте HELP M|F|L|S|Q|T - показать справку по конкретной структуре" << endl;
        }
    }

//====================================================== CLEAR =================================================================

    else if (cmd == "CLEAR") {
        if (currentArray) { 
            if (currentArray->data) delete[] currentArray->data; 
            delete currentArray; 
            currentArray = nullptr; 
        }
        if (currentList) { 
            FDelList(*currentList); 
            delete currentList; 
            currentList = nullptr; 
        }
        if (currentDList) { 
            DLDelAll(*currentDList); 
            delete currentDList; 
            currentDList = nullptr; 
        }
        if (currentStack) { 
            SDelAll(currentStack); 
            delete currentStack; 
            currentStack = nullptr; 
        }
        if (currentQueue) { 
            delete currentQueue; 
            currentQueue = nullptr; 
        }
        if (currentTree) { 
            clearTree(currentTree->root); 
            delete currentTree; 
            currentTree = nullptr; 
        }
        cout << "Все структуры данных очищены" << endl;
    }

//============================================================= EXIT =====================================================================

    else if (cmd == "EXIT") {
        if (currentArray) { 
            if (currentArray->data) delete[] currentArray->data; 
            delete currentArray; 
        }
        if (currentList) { 
            FDelList(*currentList); 
            delete currentList; 
        }
        if (currentDList) { 
            DLDelAll(*currentDList); 
            delete currentDList; 
        }
        if (currentStack) { 
            SDelAll(currentStack); 
            delete currentStack; 
        }
        if (currentQueue) { 
            delete currentQueue; 
        }
        if (currentTree) { 
            clearTree(currentTree->root); 
            delete currentTree; 
        }
        cout << "Выход из программы..." << endl;
        exit(0);
    }
    else {
        cout << "Неизвестная команда: " << cmd << ". Введите HELP для справки." << endl;
    }
}


void saveArrayToFile(Array& arr, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "ARRAY_SIZE:" << MGetSize(arr) << endl;
        for (int i = 0; i < MGetSize(arr); i++) {
            file << MGetValue(arr, i) << endl;
        }
        file.close();
        cout << "Массив сохранен в " << filename << endl;
    } else {
        cout << "Ошибка сохранения массива" << endl;
    }
}

void loadArrayFromFile(Array& arr, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        
        if (arr.data) {
            delete[] arr.data;
        }
        arr.data = nullptr;
        arr.size = 0;
        arr.capacity = 0;
        
        MCreate(arr);
        
        string line;
        getline(file, line);
        
        while (getline(file, line)) {
            if (!line.empty()) {
                MPushBack(arr, line);
            }
        }
        file.close();
        cout << "Массив загружен из " << filename << endl;
    } else {
        cout << "Ошибка загрузки массива из " << filename << endl;
    }
}

void saveListToFile(SingleLL& list, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "LIST_SIZE:" << FGetSize(list) << endl;
        for (int i = 0; i < FGetSize(list); i++) {
            string value = FGetIndex(list, i);
            if (!value.empty()) {
                file << value << endl;
            }
        }
        file.close();
        cout << "Список сохранен в " << filename << endl;
    } else {
        cout << "Ошибка сохранения списка" << endl;
    }
}

void loadListFromFile(SingleLL* list, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        if (list != nullptr){
            FDelList(*list);
            delete list;
        }
        list = new SingleLL{nullptr, 0};
        
        string line;
        getline(file, line);
        
        while (getline(file, line)) {
            if (!line.empty()) {
                FPushBack(*list, line);
            }
        }
        file.close();
        cout << "Список загружен из " << filename << endl;
    } else {
        cout << "Ошибка загрузки списка" << endl;
    }
}

void saveStackToFile(Stack* stack, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "STACK_SIZE:" << SGetSize(stack) << endl;
        Stack temp;
        CreateS(temp);
        temp.size = SGetSize(stack);
        
        while (!isEmpty(stack)) {
            string value = SPop(stack);
            file << value << endl;
            SPush(&temp, value);
        }
        
        while (!isEmpty(&temp)) {
            SPush(stack, SPop(&temp));
        }
        
        SDelAll(&temp);
        file.close();
        cout << "Стек сохранен в " << filename << endl;
    } else {
        cout << "Ошибка сохранения стека" << endl;
    }
}

void loadStackFromFile(Stack*& stack, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        if (stack != nullptr) {
            SDelAll(stack);
            delete stack;
        }

        stack = new Stack;
        CreateS(*stack);

        string line;
        getline(file, line);
        
        while (getline(file, line)) {
            if (!line.empty()) {
                SPush(stack, line);
            }
        }
        file.close();
        cout << "Стек загружен из " << filename << endl;
    } else {
        cout << "Ошибка загрузки стека" << endl;
    }
}

void saveQueueToFile(Queue* queue, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "QUEUE_SIZE:" << QGetSize(*queue) << endl;
        QNode* current = queue->front;
        while (current != nullptr) {
            file << current->key << endl;
            current = current->next;
        }
        file.close();
        cout << "Очередь сохранена в " << filename << endl;
    } else {
        cout << "Ошибка сохранения очереди" << endl;
    }
}

void loadQueueFromFile(Queue*& queue, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        if (queue) QDelAll(queue);
        queue = new Queue;
        CreateQ(*queue);
        
        string line;
        getline(file, line);
        
        while (getline(file, line)) {
            if (!line.empty()) {
                QPush(*queue, line);
            }
        }
        file.close();
        cout << "Очередь загружена из " << filename << endl;
    } else {
        cout << "Ошибка загрузки очереди" << endl;
    }
}

void saveDoubleListToFile(DoubleList* list, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "DLIST_SIZE:" << DLGetSize(*list) << endl;
        DLNode* current = list->head;
        for (int i = 0; current != nullptr && i < list->size; i++) {
            file << current -> key << endl;
            current = current -> next;
        }
        file.close();
        cout << "Двусвязный список сохранен в " << filename << endl;
    } else {
        cout << "Ошибка сохранения двусвязного списка" << endl;
    }
}

void loadDoubleListFromFile(DoubleList*& list, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        if (list) DLDelAll(*list);
        
        list = new DoubleList;
        CreateDL(*list);
        
        string line;
        getline(file, line);
        
        while (getline(file, line)) {
            if (!line.empty()) {
                DLPushBack(*list, line);
            }
        }
        file.close();
        cout << "Двусвязный список загружен из " << filename << endl;
    } else {
        cout << "Ошибка загрузки двусвязного списка" << endl;
    }
}

void saveTreeToFile(Tree* tree, const string& filename) {
    ofstream file(filename);
    if (file.is_open() && tree) {
        file << "TREE_SIZE:" << tree->size << endl;
        saveNodeToFile(tree->root, file);
        file.close();
        cout << "Дерево сохранено в " << filename << endl;
    } else {
        cout << "Ошибка сохранения дерева" << endl;
    }
}

void loadTreeFromFile(Tree*& tree, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        if (tree) {
            deleteTree(tree);
            delete tree;
        }

        tree = new Tree;
        initTree(tree);

        string line;
        getline(file, line); //пропускаем "TREE_SIZE:X"

        tree->root = loadNodeFromFile(file);
        
        // Подсчет размера
        queue<TreeNode*> q;
        if (tree->root) q.push(tree->root);
        tree->size = 0;
        
        while (!q.empty()) {
            TreeNode* current = q.front();
            q.pop();
            tree->size++;
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }

        file.close();
        cout << "Дерево загружено из " << filename << endl;
    } else {
        cout << "Ошибка загрузки дерева" << endl;
    }
}

void printMain() {
    cout << "\n=== СПРАВКА ПО КОМАНДАМ ===" << endl;
    
    cout << "\nОБЩИЕ КОМАНДЫ:" << endl;
    cout << "  HELP ALL - показать справку по всем структурам" << endl;
    cout << "  HELP M|F|L|S|Q|T - показать справку по конкретной структуре" << endl;
    cout << "  CLEAR - очистить все структуры" << endl;
    cout << "  EXIT - выход из программы" << endl;
    cout << "=================================" << endl;
}

void printAll() {
    cout << "\n=== СПРАВКА ПО КОМАНДАМ ===" << endl;
    cout << "\nОСНОВНЫЕ КОМАНДЫ:" << endl;
    printM();
    printF();
    printL();
    printS();
    printQ();
    printT();
    cout << "=================================" << endl;
}

void printM() {
    cout << "МАССИВ (M):" << endl;
    cout << "  MCREATE - создать массив" << endl;
    cout << "  MPUSH F|B|index \"value\" - добавить элемент" << endl;
    cout << "  MDEL F|B|V|index - удалить элемент" << endl;
    cout << "  MGET F|B|index - получить элемент" << endl;
    cout << "  MFIND \"value\" - найти элемент" << endl;
    cout << "  MSIZE - получить размер" << endl;
    cout << "  MPRINT - вывести массив" << endl;
    cout << "  MREPLACE index value - заменить значение по индексу" << endl;
    cout << "  MSAVE filename - сохранить в файл" << endl;
    cout << "  MLOAD filename - загрузить из файла" << endl;
}

void printF() {
    cout << "\nОДНОСВЯЗНЫЙ СПИСОК (F):" << endl;
    cout << "  FCREATE - создать список" << endl;
    cout << "  FPUSH F|B|AFT|BEF|index \"value\" - добавить элемент" << endl;
    cout << "  FDEL F|B|V|AFT|BEF|index - удалить элемент" << endl;
    cout << "  FGET F|B|index - получить элемент" << endl;
    cout << "  FFIND \"value\" - найти элемент" << endl;
    cout << "  FPRINT - вывести список" << endl;
    cout << "  FSAVE filename - сохранить в файл" << endl;
    cout << "  FLOAD filename - загрузить из файла" << endl;
}

void printL() {
    cout << "\nДВУСВЯЗНЫЙ СПИСОК (L):" << endl;
    cout << "  LCREATE - создать список" << endl;
    cout << "  LPUSH F|B|AFT|BEF|index \"value\" - добавить элемент" << endl;
    cout << "  LDEL F|B|V|AFT|BEF|index \"target\" - удалить элемент" << endl;
    cout << "  LGET F|B|index - получить элемент" << endl;
    cout << "  LPRINT F|B - вывести список" << endl;
    cout << "  LSAVE filename - сохранить в файл" << endl;
    cout << "  LLOAD filename - загрузить из файла" << endl;
}

void printS() {
    cout << "\nСТЕК (S):" << endl;
    cout << "  SCREATE - создать стек" << endl;
    cout << "  SPUSH \"value\" - добавить элемент" << endl;
    cout << "  SPOP - удалить элемент" << endl;
    cout << "  STOP - посмотреть верхний элемент" << endl;
    cout << "  SPRINT - вывести стек" << endl;
    cout << "  SSAVE filename - сохранить в файл" << endl;
    cout << "  SLOAD filename - загрузить из файла" << endl;
}

void printQ() {
    cout << "\nОЧЕРЕДЬ (Q):" << endl;
    cout << "  QCREATE - создать очередь" << endl;
    cout << "  QPUSH \"value\" - добавить элемент" << endl;
    cout << "  QPOP - удалить элемент" << endl;
    cout << "  QINDEX \"value\" - посмотреть элемент по индексу" << endl;
    cout << "  QPRINT - вывести очередь" << endl;
    cout << "  QSAVE filename - сохранить в файл" << endl;
    cout << "  QLOAD filename - загрузить из файла" << endl;
}

void printT() {
    cout << "\nДЕРЕВО (T):" << endl;
    cout << "  TCREATE - создать дерево" << endl;
    cout << "  TINSERT value - добавить элемент (число)" << endl;
    cout << "  TFIND value - найти элемент" << endl;
    cout << "  TPRINT - вывести дерево (4 вида: 3 вида  DFS и BFS)" << endl;
    cout << "  TREMOVE \"value\" - удалить последний узел" << endl;
    cout << "  TSAVE filename - сохранить в файл" << endl;
    cout << "  TLOAD filename - загрузить из файла" << endl;
}