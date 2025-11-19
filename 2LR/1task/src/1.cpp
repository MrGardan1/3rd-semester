#include "infixToPostfix.h"
#include <iostream>
using namespace std;

int main() {

    const char* tests[] = {
        "A+B*C",
        "(A+B)*C",
        "3+4*2/(1-5)^2^3",
        "x1 + x2+(y - 7)"
    };
    for (auto s : tests) {
        std::string r = infixToPostfix(s);
        if (r.empty()) {
            std::cout << s << " -> ERROR\n";
        } else {
            std::cout << s << " -> " << r << "\n";
        }
    }
    return 0;
}