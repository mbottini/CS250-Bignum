#include <iostream>
#include "bignum.h"

// HighestNumAllowed = 2^500, defined in bignum.h.


int main() {
    BigNum a;
    BigNum b;
    string inputStr;

    std::cout << "Input the first number.\n";
    std::cin >> inputStr;
    if(abs(BigNum(inputStr)) < HighestNumAllowed) {
        a = BigNum(inputStr);
    }

    else {
        std::cout << "Input is too large of a number.\n";
        return 0;
    }

    std::cout << "Input the second number.\n";
    std::cin >> inputStr;
    if(abs(BigNum(inputStr)) < HighestNumAllowed) {
        b = BigNum(inputStr);
    }

    else {
        std::cout << "Input is too large of a number.\n";
        return 0;
    }

    BigNum quotient(0);
    BigNum remainder(0);

    QuickDivide(a, b, quotient, remainder);

    std::cout << quotient << "\n";
    std::cout << remainder << "\n";
    return 0;
}

