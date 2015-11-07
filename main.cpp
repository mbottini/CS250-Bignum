#include <iostream>
#include "bignum.h"

const BigNum HighestNumAllowed =
BigNum("3273390607896141870013189696827599152216642046043064789483291368096133"
"79640467455488327009232590415715088668412756007100921725654588539305332"
"8527589376");

int main() {
    BigNum a("31415926534676736647");
    BigNum b("438478473847834834784748");
    BigNum c;
    BigNum d;

    Division(b, a, c, d);
    std::cout << b << " / " << a << " = " << c << " remainder " << d << "\n";
    std::cout << "GCD = " << gcd(a, b) << "\n";
    return 0;
}

