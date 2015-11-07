#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::string string;
typedef std::ostream ostream;
typedef std::vector<int> intVector;
typedef std::vector<int>::iterator intIter;
typedef std::vector<int>::const_iterator intConstIter;
typedef std::vector<int>::reverse_iterator intRevIter;
typedef std::vector<int>::const_reverse_iterator intRevConstIter;

class BigNum {
    private:
        intVector _array;
        bool _negative;

    public:
        BigNum();
        BigNum(const string& inputStr);
        BigNum(const BigNum& original);
        BigNum& operator =(const BigNum& original);
        BigNum(const intVector& v, bool neg);

        int operator [](int index) const;
        int length() const;
        bool isNegative() const;
        const intVector& getArray() const;

        BigNum operator +(const BigNum& otherNum) const;
        BigNum operator -(const BigNum& otherNum) const;
        BigNum& operator -=(const BigNum& otherNum);
        BigNum& operator ++(int);
        BigNum operator -() const;

        bool operator ==(const BigNum& otherNum) const;
        bool operator >(const BigNum& otherNum) const;
        bool operator <(const BigNum& otherNum) const;
        bool iszero() const;

};

BigNum gcd(BigNum bn1, BigNum bn2);
void Division(BigNum dividend, const BigNum& divisor, BigNum& quotient, 
              BigNum& remainder);
intVector stripTrailingZeros(intVector v);
ostream& operator <<(ostream& os, const BigNum& bn);
void printVector(const intVector& v);
