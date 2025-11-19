#include "infixToPostfix.h"

bool isIdentChar(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return isalnum(uc) || c=='_';
}

int precedence(char op) {
    switch(op) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

bool isRightAssoc(char op) {
    return op == '^';
}

bool isOperator(char c) {
    return c=='+' || c== '-'|| c=='*'|| c=='/'|| c=='^';
}

static void push_op(Stack& ops, std::string& out, char cur) {
    char t;
    while (ops.top(t) && isOperator(t)) {
        int pt = precedence(t), pc = precedence(cur);
        //если оператор в стеке имеет больший приоритет
        //или приоритет одинаковый и текущий оператор не степень,
        // то извлекаем оператор из стека и добавляем в строку выходную
        bool popIt = (pt > pc) || (pt == pc && !isRightAssoc(cur));
        if (popIt) {
            ops.pop(t);
            out += t; out += ' ';
        } else break;
    }
    //закидываем текущий оператор
    ops.push(cur);
}

std::string infixToPostfix(const std::string& s) {
    Stack ops;
    std::string out;

    for (size_t i=0;i<s.size();) {
        unsigned char uc = static_cast<unsigned char>(s[i]);
        if (std::isspace(uc)) { ++i; continue; }// пропуск пробелов

        //считывание и запись в выходную строку коэф и переменные
        if (std::isalnum(uc)) {
            size_t j = i + 1;
            while (j < s.size() && std::isalnum(static_cast<unsigned char>(s[j]))) {
                ++j;
            }
            out.append(s, i, j - i);
            out.push_back(' ');
            i = j;
            continue;
        }

        // обработка скобок
        if (s[i]=='(') { ops.push('('); ++i; continue; } // левую скобку в стек 
        if (s[i]==')') {
            char t;
            while (ops.top(t) && t!='(') { // выталкивать до '('
                ops.pop(t);
                out += t; out += ' ';
            }
            if (!ops.top(t) || t!='(') return std::string(); // непарные скобки
            ops.pop(t); // убрать '('
            ++i; continue;
        }

        // операторы
        if (isOperator(s[i])) {
            push_op(ops, out, s[i]); // применить правило приоритета/ассоциативности
            ++i; continue;
        }

        ++i;
    }

    // сброс оставшегося
    char t;
    while (ops.pop(t)) {
        if (t=='(' || t==')') return std::string(); // непарные скобки
        out += t; out += ' ';
    }
    if (!out.empty() && out.back()==' ') out.pop_back();
    return out;
}