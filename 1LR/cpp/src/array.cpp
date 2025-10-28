#include "array.h"
const int MAX_CAPACITY = 1000;


void MCreate(Array& arr) {
    int initCapacity = 10;
    arr.data = new string[initCapacity]();
    arr.size = 0;
    arr.capacity = initCapacity;
}

void MResize(Array& arr) {
    if (arr.size >= arr.capacity) {
        int newCapacity = arr.capacity * 2;
        string* newData = new string[newCapacity];
        
        for (int i = 0; i < arr.size; i++) {
            newData[i] = arr.data[i];
        }
        
        delete[] arr.data;
        arr.data = newData;
        arr.capacity = newCapacity;
    }
}

void MDelIndex(Array& arr, int index) {
    if (index >= arr.size || index < 0) {
        cout << "Error: Out of range" << endl;
        return;
    }
    for (int i = index; i < arr.size-1; i++) {
        arr.data[i] = arr.data[i+1];
    }
    arr.size--;
}

void OutputM(Array& arr) {
    cout << "Array ["<< arr.size <<"]: ";
    for (int i = 0; i < arr.size; i++) {
        cout << arr.data[i] << " ";
    }
    if (arr.size  == 0) {
        cout << "Empty array";
    }
    cout << endl;
}

void OutputM(Array& arr, int index) {
    cout << "Array element [" << index <<"] = " << arr.data[index] << endl;
}

void MInsert(Array& arr, int index, string key) {
    if (index >= MAX_CAPACITY) {
        cout << "Too large index" << endl;
        return;
    }
    if (index < 0) {
        cout << "Error: incorrect index" << endl;
        return;
    }
    
    if (index >= arr.capacity) {
        int newCapacity = arr.capacity;
        while (index >= newCapacity) {
            newCapacity = newCapacity * 2;
        }
        arr.capacity = newCapacity;
    }
    
    if (index > arr.size) {
        for (int i = arr.size; i < index; i++) {
            arr.data[i] = "";
        }
        arr.data[index] = key;
        arr.size = index + 1;  
    } else {
        for (int i = arr.size; i > index; i--) {
            arr.data[i] = arr.data[i - 1];
        }
        arr.data[index] = key;
        arr.size++;
    }
}

void MReplace(Array& arr, int index, string key) {
    MInsert(arr, index, key);
}

void MPushBack(Array& arr, string key) {
    if (arr.size >= arr.capacity) {
        MResize(arr);
    }
    arr.data[arr.size] = key;
    arr.size++;
}

int MGetSize(Array& arr) {
    return arr.size;
}

string MGetValue(Array& arr, int index) {
    if (index <0 || index >= arr.size) {
        cout << "Out of range" << endl;
        return "";
    }
    return arr.data[index];
}