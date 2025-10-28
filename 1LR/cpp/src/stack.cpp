#include "stack.h"

void CreateS(Stack& st) {
    int initCapacity = 10;
    st.data = new string[initCapacity];
    st.size = 0;
    st.capacity = initCapacity;
}

void OutputS(Stack& st) {
    cout << "Stack[" << st.size << "]: ";
    for (int i = 0; i<st.size; i++) {
        cout << st.data[i] << " ";
    }
    cout << endl;
}

void SResize(Stack& st) {
    if (st.size >= st.capacity - 1) {
        st.capacity *= 2;
        string* newData = new string[st.capacity];
        for (int i = 0; i < st.size; i++) {
            newData[i] = st.data[i];
        }
        delete[] st.data;
        st.data = newData;
    }
}

void SPush(Stack* st, string key) {
    SResize(*st);
    st->data[st->size] = key;
    st->size++;
}

string SPop(Stack* st) {
    if (st->size == 0) {
        cout << "Error: Stack is empty" << endl;
        return "";
    }
    string temp = st->data[st->size-1];
    st->size--;
    return temp;
}

void SDelAll(Stack* st) {
    if (st != nullptr) {
        delete[] st->data;
        st->data = nullptr;
        st->size = 0;
        st->capacity = 0;
    }
}

string SPeek(Stack* st) {
    if(isEmpty(st)){
        cout << "Stack is empty" << endl;
        return "";
    }
    return st->data[st->size];
}

int SGetSize(Stack* st) {
    return st->size;
}

bool isEmpty(Stack* st) {
    return st->size == 0;
}

bool isFull(Stack& st) {
    return st.size == st.capacity - 1;
}

