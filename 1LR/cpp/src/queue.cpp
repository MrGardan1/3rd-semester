#include "queue.h"

void CreateQ(Queue& q) {
    q.front = nullptr;
    q.back = nullptr;
    q.size = 0;
}

void OutputQ(Queue& q) {
    
    cout << "Queue [" << q.size << "]: ";
    if (q.front == nullptr) {
        cout << "Error: Empty Queue";
        cout << endl;
        return;
    }
    QNode* current = q.front;
    for (int i = 0; i < q.size; i++) {
        cout << current -> key << " ";
        current = current -> next;
    }
    cout << endl;

}

void QPush(Queue& q, string key) {
    QNode* newNode = new QNode{key, nullptr, nullptr};

    if (q.back == nullptr) {
        q.front = newNode;
        q.back = newNode;
    } else {
        newNode-> past = q.back;
        q.back -> next = newNode;
        q.back = newNode;
    }
    q.size++;
}

void QPop (Queue& q) {
    if (q.front == nullptr) {
        cout << "Error: Queue is empty" << endl;
        return;
    }
    QNode* deleteNode = q.front;
    q.front = q.front ->next;

    if (q.front != nullptr) {
        q.front -> past = nullptr;
    } else {
        q.back = nullptr;
    }
    delete deleteNode;
    q.size--;
}

bool isEmpty(Queue* queue) {
    return queue->size == 0;
}



int QGetSize(Queue& q) {
    return q.size;
}

string QGetIndex(Queue& q, int index) {
    if (q.size == 0) {
        return "";
    }
    if (index <0 || index >q.size) {
        cout << "Out of range" << endl;
        return "";
    }
    QNode* current = q.front;
    int currentIndex = 0;
    while (current != nullptr && currentIndex < index) {
        current = current -> next;
        currentIndex++;
    }

    if (current != nullptr) {
        return current->key;
    }
    return  "";
}

void QDelAll(Queue* q) {
    if (q != nullptr) {
        while (q -> front != nullptr) {
            QNode* temp = q->front;
            q-> front = q-> front -> next;
            delete temp;
        }
        delete q;
    }
}