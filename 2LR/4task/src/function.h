#pragma once

#include <iostream>
#include <string>
using namespace std;

class Array {
 public:
    Array(int size);
    ~Array();
    void insert(string& s);
    string get(int index);
    int size();
 private:
    int size_;
    int capacity_ = 100;
    string* data;
};

bool matchHelper(const std::string& s, const std::string& pattern, int si, int pi);
bool isEquivalent(const string& s,const string& pattern);