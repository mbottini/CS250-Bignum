#include "bignum.h"

BigNum::BigNum() {
    _array.push_back(0);
    _negative = false;
    return;
}

BigNum::BigNum(const string& inputStr) {
    // The 0th character is special, as it can be either a digit or a negative.

    if(inputStr[0] == '-') {
        _negative = true;

        if(inputStr.length() == 1) {
            BigNum();
            return;
        }
    }

    else if(!isdigit(inputStr[0])) {
        BigNum();
        return;
    }

    // Scan all characters for an invalid character.

    for(int i = 1; i < inputStr.length(); i++) {
        if(!isdigit(inputStr[i])) {
            BigNum();
            return;
        }
    }

    // Now that we've established that all of the characters are numeric,
    // we can add them to the vector.

    _array.reserve(inputStr.length());

    // Least significant digit goes into the 0th position in the vector.
    for(int i = inputStr.length() - 1; i > 0; i--) {
        _array.push_back(inputStr[i] - '0');
    }

    if(isdigit(inputStr[0])) {
        _array.push_back(inputStr[0] - '0');
    }

    _array = stripTrailingZeros(_array);
    return;
}

BigNum::BigNum(const BigNum& original) {
    *this = original;
    return;
}

BigNum& BigNum::operator =(const BigNum& original) {
    _array = original.getArray();
    _array = stripTrailingZeros(_array);
    _negative = original.isNegative();
    return *this;
}

BigNum::BigNum(const intVector& v, bool neg) {
    _array = v;
    _array = stripTrailingZeros(_array);
    _negative = neg;
    return;
}

int BigNum::operator [](int index) const {
    if(index >= 0 && index < _array.size()) {
        return _array[index];
    }

    else {
        return 0;
    }
}

int BigNum::length() const {
    return _array.size();
}

bool BigNum::isNegative() const {
    return _negative;
}

const intVector& BigNum::getArray() const {
    return _array;
}

BigNum BigNum::operator +(const BigNum& otherNum) const {
    if(_negative && otherNum.isNegative()) {
        return -(-*this + -otherNum);
    }

    else if(_negative) {
        return otherNum - (-*this);
    }

    else if(otherNum.isNegative()) {
        return *this - (-otherNum);
    }

    int biggestLength = (this->length() > otherNum.length()) ?
                              this->length() : otherNum.length();

    intVector newVector;
    newVector.reserve(biggestLength + 1);

    bool carry = false;
    int newNum;

    for(int i = 0; i < biggestLength; i++) {
        newNum = (*this)[i];

        if(carry) {
            newNum++;
            carry = false;
        }

        newNum += otherNum[i];

        if(newNum > 9) {
            newNum -= 10;
            carry = true;
        }

        newVector.push_back(newNum);
    }

    if(carry) {
        newVector.push_back(1);
    }

    return BigNum(newVector, false);
}

BigNum BigNum::operator -(const BigNum& otherNum) const {
    if(_negative && otherNum.isNegative()) {
        return -(-*this - otherNum);
    }

    else if(_negative) {
        return -(-*this + otherNum);
    }

    else if(otherNum.isNegative()) {
        return *this + -otherNum;
    }

    else if(otherNum > *this) {
        return -(otherNum - *this);
    }

    intVector newVector;
    newVector.reserve(this->length());

    bool carry = false;
    int newNum;

    for(int i = 0; i < this->length(); i++) {
        newNum = (*this)[i];

        if(carry) {
            newNum--;
            carry = false;
        }

        newNum -= otherNum[i];

        if(newNum < 0) {
            newNum += 10;
            carry = true;
        }

        newVector.push_back(newNum);
    }

    return BigNum(newVector, false);
}

BigNum& BigNum::operator -=(const BigNum& otherNum) {
    *(this) = *(this) - otherNum;
    return *this;
}

BigNum& BigNum::operator ++(int) {
    bool carry = true;

    for(intIter i = _array.begin(); i != _array.end() && carry; ++i) {
        carry = false;
        (*i)++;
        if((*i) > 9) {
            (*i) -= 10;
            carry = true;
        }
    }

    if(carry) {
        _array.push_back(1);
    }

    return *this;
}

BigNum BigNum::operator -() const {
    return BigNum(_array, !_negative);
}

bool BigNum::operator ==(const BigNum& otherNum) const {
    int biggestLength = (this->length() > otherNum.length()) ?
                              this->length() : otherNum.length();

    for(int i = 0; i < biggestLength; i++) {
        if((*this)[i] != otherNum[i]) {
            return false;
        }
    }

    return true;
}

bool BigNum::operator >(const BigNum& otherNum) const {
    if(otherNum.length() > this->length()) {
        return false;
    }

    if(this->length() > otherNum.length()) {
        return true;
    }

    // Otherwise, they're equal in length, and we have to compare each digit,
    // starting at the most significant digit.

    for(int i = this->length() - 1; i >= 0; i--) {
        if((*this)[i] != otherNum[i]) {
            return (*this)[i] > otherNum[i];
        }
    }

    // Otherwise, they're equal.
    return false;
}

bool BigNum::operator <(const BigNum& otherNum) const {
    return !(*this == otherNum) && !(*this > otherNum);
}

bool BigNum::iszero() const {
    if(this->length() == 1) {
        return (*this)[0] == 0;
    }

    else {
        return false;
    }
}


BigNum gcd(BigNum bn1, BigNum bn2) {
    if(bn1.iszero() || bn2.iszero()) {
        return BigNum();
    }

    if(bn2 > bn1) {
        std::swap(bn1, bn2);
    }

    if((bn2 - bn1).iszero()) {
       return bn2;
    }

    else {
       return gcd(bn2, bn1 - bn2);
    }
} 

void Division(BigNum dividend, const BigNum& divisor, BigNum& quotient,
              BigNum& remainder) {
    if(divisor.iszero()) {
        quotient = BigNum();
        remainder = BigNum();
        return;
    }

    quotient = BigNum();

    while(dividend > divisor) {
        dividend -= divisor;
        quotient++;
    }

    remainder = dividend;
    return;
}

intVector stripTrailingZeros(intVector v) {
    int j = 0;
    for(intRevIter i = v.rbegin(); i != v.rend() && *i == 0; ++i) {
        j++;
    }
    
    for(; j > 0; j--) {
        v.pop_back();
    }

    if(v.empty()) {
        v.push_back(0);
    }

    return v;
}


ostream& operator <<(ostream& os, const BigNum& bn) {
    if(bn.isNegative()) {
        os << "-";
    }

    for(intRevConstIter j = bn.getArray().rbegin(); 
                        j != bn.getArray().rend(); j++) {
        os << *j;
    }

    return os;
}

void printVector(const intVector& v) {
    for(intConstIter i = v.begin(); i != v.end(); ++i) {
        std::cout << *i;
    }
    std::cout << "\n";
    return;
}
