#include <iostream>
#include "bignum.h"

// HighestNumAllowed = 2^500, defined in bignum.h.


int main() {
    BigNum a;
    BigNum b;
    string inputStr;

    std::cout << "Input the first number.\n";
    std::cin >> inputStr;
    while(BigNum(inputStr).isNaN() || BigNum(inputStr) > HighestNumAllowed ||
                                      BigNum(inputStr).isNegative()) {
        if(BigNum(inputStr).isNaN()) {
            std::cout << "Input is not a number.\n";
        }

        else {
            std::cout << "Input is too or too small of a number.\n";
        }

        std::cout << "Input the first number.\n";
        std::cin >> inputStr;
    }

    a = BigNum(inputStr);


    std::cout << "Input the second number.\n";
    std::cin >> inputStr;

    while(BigNum(inputStr).isNaN() || BigNum(inputStr) > HighestNumAllowed ||
                                      BigNum(inputStr).isNegative()) {

        if(BigNum(inputStr).isNaN()) {
            std::cout << "Input is not a number.\n";
        }

        else {
            std::cout << "Input is too large or too small of a number.\n";
        }

        std::cout << "Input the second number.\n";
        std::cin >> inputStr;
    }

    b = BigNum(inputStr);

    BigNum quotient;
    BigNum remainder;

    QuickDivide(a, b, quotient, remainder);

    std::cout << a << " / " << b << " = " << quotient << ", remainder " <<
                 remainder << "\n";
    std::cout << "GCD = " << gcd(a, b) << "\n";
    return 0;
}

