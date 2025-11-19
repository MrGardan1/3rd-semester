#include "function.h"

Array::Array(int capacity) : capacity_(capacity), size_(0) {
        data = new std::string[capacity_];
}

Array::~Array() {
    delete[] data;
}

void Array::insert(string& s) {
    if (size_ < capacity_) {
        data[size_] = s;
        size_++;
    } else {
        cout << "Error: Нужно расширить массив" << endl;
    }
}

string Array::get(int index) {
    if (index < 0 || index > capacity_) {
        throw runtime_error("Error: out of range");
    }

    return data[index];
}

int Array::size() {
    return size_;
}

bool matchHelper(const string& s, const string& pattern, int si, int pi) {
    if (pi == pattern.size()) {
        return si == s.size();//закончился шаблон, проверка закончилась ли строка
    }
    if (pattern[pi] == '*') {
        //пробуем заменить * на пустую строку
        if (matchHelper(s, pattern, si, pi + 1)) {
            return true;
        }
        //пробуем сопоставить * с текущим символом строки, если есть
        if (si < s.size() && matchHelper(s, pattern, si + 1, pi)) {
            return true;
        }
        return false;
    }
    else if (pattern[pi] == '?') {
        return (si < s.size()) && matchHelper(s, pattern, si + 1, pi + 1);
    }
    else {
        //без * и ?
        return (si < s.size() && s[si] == pattern[pi]) && matchHelper(s, pattern, si + 1, pi + 1);
    }
}

bool isEquivalent(const string& s, const string& pattern) {
    return matchHelper(s, pattern, 0, 0);
}