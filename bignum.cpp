#include "bignum.h"

BigNum::BigNum() {
    _array.push_back(-1); // Negative number in the first element means NaN.
    _negative = false;
    return;
}

BigNum::BigNum(int inputInt) {
    std::stringstream ss;
    ss << inputInt;
    string inputStr = ss.str();

    // Duplicate of below. Unfortunately, C++ doesn't allow calling one
    // constructor inside another.

    if(inputStr[0] == '-') {
        _negative = true;
        inputStr.erase(0, 1);
    }

    else {
        _negative = false;
    }

    _array = ParseString(inputStr);
    _array = stripTrailingZeros(_array);
    return;
}

BigNum::BigNum(string inputStr) {
    if(inputStr[0] == '-') {
        _negative = true;
        inputStr.erase(0, 1);
    }

    else {
        _negative = false;
    }


    _array = ParseString(inputStr);
    _array = stripTrailingZeros(_array);
    return;
}

BigNum::BigNum(const BigNum& original) {
    *this = original;
    return;
}

BigNum& BigNum::operator =(const BigNum& original) {
    if(original.isNaN()) {
        BigNum();
        return *this;
    }

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
    if(!(this->isNaN()) && index >= 0 && index < _array.size()) {
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
    if(this->isNaN() || otherNum.isNaN()) {
        return BigNum();
    }

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
    if(this->isNaN() || otherNum.isNaN()) {
        return BigNum();
    }

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

BigNum BigNum::operator /(const BigNum& otherNum) const {
    if(this->isNaN() || otherNum.isNaN()) {
        return BigNum();
    }

    if(this->isNegative() && otherNum.isNegative()) {
        return (-(*this)) / (-(otherNum));
    }

    else if(this->isNegative()) {
        return -((-(*this)) / otherNum);
    }

    else if(otherNum.isNegative()) {
        std::cout << "Returning " << (*this) << " / " << -(otherNum) << "\n";
        return -((*this) / (-(otherNum)));
    }

    if(!otherNum.iszero()) {
        BigNum temp = *this;
        BigNum quotient(0);

        while(temp > otherNum) {
            temp -= otherNum;
            quotient++;
        }

        return quotient;
    }

    else {
        return BigNum();
    }
}

BigNum BigNum::operator %(const BigNum& otherNum) const {
    if(this->isNaN() || otherNum.isNaN()) {
        return BigNum();
    }

    if(otherNum.isNegative()) { // Modulus must be positive.
        return BigNum();
    }

    if(this->isNegative()) {
        return otherNum - (-(*this) % otherNum);
    }
    
    if(!otherNum.iszero()) {
        BigNum temp = *this;
        
        while(temp > otherNum) {
            temp -= otherNum;
        }

        return temp;
    }

    else {
        return BigNum();
    }
}

BigNum BigNum::operator +=(const BigNum& otherNum) {
    *(this) = *(this) + otherNum;
    return *this;
}

BigNum& BigNum::operator -=(const BigNum& otherNum) {
    *(this) = *(this) - otherNum;
    return *this;
}

BigNum& BigNum::operator <<=(const int n) {
    for(int i = 0; i < n; i++) {
        _array.insert(_array.begin(), 0);
    }
    return *this;
}

BigNum& BigNum::operator ++(int) {
    if(this->isNaN()) {
        return *this;
    }

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
    if(this->isNaN()) {
        return BigNum();
    }
    return BigNum(_array, !_negative);
}

bool BigNum::operator ==(const BigNum& otherNum) const {
    if(this->isNaN() || otherNum.isNaN()) {
        return false;
    }
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
    if(this->isNaN() || otherNum.isNaN()) {
        return false;
    }

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
    if(this->isNaN()) {
        return false;
    }

    if(this->length() == 1) {
        return (*this)[0] == 0;
    }

    else {
        return false;
    }
}

bool BigNum::isNaN() const {
    return _array[0] < 0;
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
       return gcd(bn2, bn1 % bn2);
    }
} 

void QuickDivide(const BigNum& dividend, const BigNum& divisor, 
        BigNum& quotient, BigNum& remainder) {
    remainder = BigNum(0);

    intVector dVec = dividend.getArray();
    intVector qVec;
    BigNum q(0);

    for(intRevConstIter i = dVec.rbegin(); i != dVec.rend(); ++i) {
        remainder <<= 1;
        remainder += (*i);
        q = remainder / divisor;
        remainder = remainder % divisor;
        qVec.push_back(q.getArray().at(0));
    }

    std::reverse(qVec.begin(), qVec.end());

    quotient = BigNum(qVec, false);
    if((divisor - remainder).iszero()) {
        remainder = BigNum(0);
        quotient++;
    }
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
    if(!bn.isNaN()) {
        if(bn.isNegative()) {
            os << "-";
        }

        for(intRevConstIter j = bn.getArray().rbegin(); 
                            j != bn.getArray().rend(); ++j) {
            os << *j;
        }
    }

    else {
        os << "NaN";
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

intVector ParseString(const string& inputStr) {
    // Check to make sure that the string has only numbers in it.
    intVector newVector;

    if(std::all_of(inputStr.begin(), inputStr.end(), ::isdigit)) {
        newVector.reserve(inputStr.length());
        for(int i = inputStr.length() - 1; i >= 0; i--) {
            newVector.push_back(inputStr[i] - '0');
        }
    }

    return newVector;
}

BigNum abs(const BigNum& bn) {
    if(bn.isNegative()) {
        return -bn;
    }

    else {
        return bn;
    }
}
