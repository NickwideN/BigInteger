#include<iostream>
#include<iomanip>
#include"BigInteger.h"

using namespace std;
int main() {
    BigInteger a = 987654321;
    BigInteger b = 987654321;
    for (;;) {
        cin >> a;
        cout << a;
    }
    a++;
    getchar();
    getchar();
    return 0;
}





/*
int main() {
    BigInteger a = "-1234561234";
    BigInteger b;
    b = "123456789112345000";
    BigInteger c = a * b;
    std::cout << a << std::endl << b << std::endl << c << std::endl << a + b << std::endl << std::endl;
    std::cout << std::endl;

    std::cout << (a > 6) << std::endl << (c < b) << std::endl << std::endl;

    BigInteger q = 11112222;
    BigInteger w = "10000000000000000000000000000000000000"; //////////////////error if q * w then return 111222200000
    std::cout << std::setw(4) << q * w;
    getchar();
    getchar();
    return 0;
}
*/