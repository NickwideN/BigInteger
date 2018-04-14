#include "BigInteger.h"
#include <cstring>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
//------------------------------------------------------------------
//      Privite friend functions:
BigInteger & Addition(BigInteger & left_value, const BigInteger & right_value) {  
                                                            // similar operator += (left_value += right_value)
                                                            // function takes left_value and right_value only as pozitive
    int new_value_len = left_value.len > right_value.len ? left_value.len : right_value.len;
    for (int i = 0; i < new_value_len; ++i) {
        left_value.digits[i] += right_value.digits[i];
        if (left_value.digits[i] >= BigInteger::BASE) {
            if (i == BigInteger::MAX_LEN_BASE - 1) {
                throw BigInteger::BigIntegerOverflow();
            }
            left_value.digits[i] -= BigInteger::BASE;
            left_value.digits[i + 1] += 1;
        }
    }
    left_value.len = (left_value.digits[new_value_len] ? new_value_len + 1 : new_value_len);
    return left_value;
}

BigInteger & Substraction(BigInteger & left_value, const BigInteger & right_value) {  
                                                            // similar operator -= (left_value -= right_value)
                                                            // function takes left_value and right_value only as pozitive
                                                            // where left_value > right_value
    for (int i = 0; i < left_value.len; ++i) {
        if (left_value.digits[i] < right_value.digits[i]) {
            left_value.digits[i] += BigInteger::BASE;
            left_value.digits[i + 1] -= 1;
        }
    }
    for (int i = left_value.len - 1; i >= 0; --i) {
        left_value.digits[i] -= right_value.digits[i];
        if (!left_value.digits[i] && !left_value.digits[i + 1]) {
            --left_value.len;
        }
    }
    return left_value;
}

BigInteger Multiplication(const BigInteger & left_value, const BigInteger & right_value) { 
                                                            // similar operator *= (left_value *= right_value)
                                                            // take left_value and right_value only as pozitive
    if (left_value.len + right_value.len > BigInteger::MAX_LEN_BASE) {
        throw BigInteger::BigIntegerOverflow();
    }
    BigInteger new_value;
    for (int i = 0; i < left_value.len; ++i) {
        for (int j = 0; j < right_value.len; ++j) {
            if (i + j == left_value.len + right_value.len - 2) {
                break;
            }
            BigInteger::long_digit_t tmp_digit = (BigInteger::long_digit_t)left_value.digits[i] * right_value.digits[j] + new_value.digits[i + j];
            new_value.digits[i + j] = tmp_digit % BigInteger::BASE;
            new_value.digits[i + j + 1] += tmp_digit / BigInteger::BASE;
        }
    }
    BigInteger::long_digit_t tmp_digit = (BigInteger::long_digit_t)left_value.digits[left_value.len - 1] * right_value.digits[right_value.len - 1] + new_value.digits[left_value.len + right_value.len -2];
    new_value.digits[left_value.len + right_value.len - 2] = tmp_digit % BigInteger::BASE;
    if (left_value.len + right_value.len + 1 > BigInteger::MAX_LEN_BASE && tmp_digit / BigInteger::BASE) {
        throw BigInteger::BigIntegerOverflow();
    }
    new_value.digits[left_value.len + right_value.len - 1] += tmp_digit / BigInteger::BASE;

    new_value.len = left_value.len + right_value.len + 1;
    while (!new_value.digits[new_value.len - 1]) {
        --new_value.len;
    }
    return new_value;
}

bool Is_abs_left_value_Fewer_abs_right_value(const BigInteger & left_value, const BigInteger & right_value) {      
                                                            // take left_value and right_value only as pozitive
    if (left_value.len != right_value.len)
        return left_value.len < right_value.len;
    else {
        for (int i = left_value.len - 1; i >= 0; --i) {
            if (left_value.digits[i] != right_value.digits[i])
                return left_value.digits[i] < right_value.digits[i];
        }
        return false;
    }
}
//------------------------------------------------------------------

//------------------------------------------------------------------
//Constructors:
BigInteger::BigInteger() {
    len = 0;
    minus = false;
    for (int i = 0; i < MAX_LEN_BASE; ++i) {
        digits[i] = default_value;
    }
}

BigInteger::BigInteger(const int & value) {
    define_this(this, abs(value));
    this->minus = (value < 0 ? true : false);
}

BigInteger::BigInteger(const unsigned int & value) {
    define_this(this, value);
}

BigInteger::BigInteger(const long long & value) {
    define_this(this, abs(value));
    this->minus = (value < 0 ? true : false);
}

BigInteger::BigInteger(const unsigned long long & value) {
    define_this(this, value);
}

BigInteger::BigInteger(const char * value_cstr) {
    minus = (value_cstr[0] == '-' ? true : false);
    const char * tmp_value_cstr = (value_cstr[0] == '-' ? value_cstr + 1 : value_cstr);
    int len_10 = strlen(tmp_value_cstr);
    if (len_10 > MAX_LEN) {
        throw BigIntegerOverflow();
    }
    int id_digits = 0;
    for (int i = len_10 - 1; i >= 0; i -= BASE_LOG10, ++id_digits) {
        char tmp_digit[BASE_LOG10];
        for (int j = i;  j > i - BASE_LOG10; --j) {
            tmp_digit[BASE_LOG10 + j - i - 1] = (j < 0 ? '0' : tmp_value_cstr[j]);
        }
        digits[id_digits] = atoi(tmp_digit);
    }
    len = id_digits;
    for (int i = len; i < BigInteger::MAX_LEN_BASE; ++i) {
        digits[i] = 0;
    }
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//   Operators:
//  12/16 ready

BigInteger operator + (const BigInteger & left_value, const BigInteger & right_value) { 
    BigInteger new_value;
    if (!(left_value.minus ^ right_value.minus)) {
        new_value = left_value;
        Addition(new_value, right_value);
        if (left_value.minus) {
            new_value.minus = !new_value.minus;
        }
        return new_value;
    } else {
        bool abs_left_value_fewer = Is_abs_left_value_Fewer_abs_right_value(left_value, right_value);
        if (abs_left_value_fewer) {
            new_value = right_value;
            Substraction(new_value, left_value);
            if (left_value.minus) {
                new_value.minus = false;
                return new_value;
            }
            else {
                new_value.minus = true;
                return new_value;
            }
        } else {
            new_value = left_value;
            Substraction(new_value, right_value);
            if (left_value.minus) {
                new_value.minus = true;
                return new_value;
            }
            else {
                new_value.minus = false;
                return new_value;
            }
        }
    }
}

BigInteger & BigInteger::operator += (const BigInteger & right_value) {
    *this = *this + right_value;
    return *this;
}

BigInteger operator - (const BigInteger & left_value, const BigInteger & right_value) {
    return left_value + -right_value;
}

BigInteger & BigInteger::operator -= (const BigInteger & right_value) {
    *this = *this - right_value;
    return *this;
}

BigInteger operator * (const BigInteger & left_value, const BigInteger & right_value) {
    BigInteger new_value = Multiplication(left_value, right_value);
    new_value.minus = left_value.minus ^ right_value.minus;
    return new_value;
}

BigInteger & BigInteger::operator *= (const BigInteger & right_value) {
    *this = *this * right_value;
    return *this;
}

BigInteger & operator % (const BigInteger & left_value, const BigInteger & right_value) {   // dodn't have
    BigInteger a;
    return a;
}

BigInteger & BigInteger::operator %= (const BigInteger & right_value) {             // dodn't have
    BigInteger a;
    return a;

}

BigInteger & operator / (const BigInteger & left_value, const BigInteger & right_value) {   // dodn't have
    BigInteger a;
    return a;
}

BigInteger & BigInteger::operator /= (const BigInteger & right_value) {             // dodn't have
    BigInteger a;
    return a;
}

BigInteger operator ^ (const BigInteger & value, const int & exponent) {
    if (exponent < 0) {
        throw BigInteger::BigIntegerError();
    }
    if (exponent == 0) {
        return 1;
    }
    BigInteger new_value = value;
    for (int i = 1; i < exponent; ++i) {
        new_value *= value;
    }
    new_value.minus = exponent % 2 & value.minus;
    new_value.len = value.len * exponent + exponent;
    while (!new_value.digits[new_value.len - 1]) {
        --new_value.len;
    }
    return new_value;
}

BigInteger & BigInteger::operator ^= (const int & exponent) {
    *this = *this ^ exponent;
    return *this;
}
//  12/16 ready

//  6/6 ready
BigInteger BigInteger::operator - () const {
    BigInteger tmp_value = *this;
    tmp_value.minus = !tmp_value.minus;
    return tmp_value;
}

BigInteger BigInteger::operator + () const {
    BigInteger tmp_value = *this;
    return tmp_value;
}

BigInteger & BigInteger::operator ++ () {
    switch (this->minus)
    {
    case false:
        if (this->digits[0] < BASE) {
            ++this->digits[0];
        } else {
            if (MAX_LEN_BASE == 1) {
                throw BigIntegerOverflow();
            }
            this->digits[0] = 0;
            ++this->digits[1];
        }
        break;
    case true:
        if (this->len && this->digits[0]) {
            --this->digits[0];
        }
        else if (this->len) {
            this->digits[0] = BigInteger::BASE - 1;
            --this->digits[1];
        }
        else {
            minus = false;
            this->digits[0] = 1;
        }
    }
    return *this;
}

BigInteger & BigInteger::operator -- () {
    this->minus = !this->minus;
    ++(*this);
    this->minus = !this->minus;
    return *this;
}

BigInteger BigInteger::operator ++ (int) {
    BigInteger tmp_value = *this;
    ++*this;
    return tmp_value;
}

BigInteger BigInteger::operator -- (int) {
    BigInteger tmp_value = *this;
    --*this;
    return tmp_value;
}
//  6/6 ready

//  6/6 ready
bool operator < (const BigInteger & left_value, const BigInteger & right_value) {
    if (!left_value.minus && !right_value.minus) {
        return Is_abs_left_value_Fewer_abs_right_value(left_value, right_value);
    }
    if (left_value.minus && right_value.minus) {
        return !Is_abs_left_value_Fewer_abs_right_value(left_value, right_value);
    }
    if (!left_value.minus && right_value.minus) {
        return false;
    }
    // if (left_value.minus && !right_value.minus)
    return true;
}

bool operator == (const BigInteger & left_value, const BigInteger & right_value) {
    if (left_value.len != right_value.len || left_value.minus != right_value.minus) {
        return false;
    }
    else {
        for (int i = left_value.len - 1; i >= 0; --i) {
            if (left_value.digits[i] != right_value.digits[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator > (const BigInteger & left_value, const BigInteger & right_value) {
    return (right_value < left_value);
}

bool operator <= (const BigInteger & left_value, const BigInteger & right_value) {
    return !(right_value < left_value);
}

bool operator >= (const BigInteger & left_value, const BigInteger & right_value) {
    return !(left_value < right_value);
}
bool operator != (const BigInteger & left_value, const BigInteger & right_value) {
    return !(left_value == right_value);
}
// 6/6 ready
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Other functions:
// 5/6 ready   (converting to int require upgrade)
std::ostream & operator << (std::ostream & os, const BigInteger & value) {
    if (value.minus) {
        if (value.len - 1 || value.digits[0]) {
            os << '-';
        }
    }
    os << value.digits[value.len - 1];
    for (int i = value.len - 2; i >= 0; --i) {
        os << std::setw(9) << std::setfill('0') << std::right << value.digits[i];
    }
    return os;
}

std::istream & operator >> (std::istream & is, BigInteger & value) {
    char value_cstr[BigInteger::MAX_LEN];
    is >> value_cstr;
    value = value_cstr;
    return is;
}

/*
std::istream & operator >> (std::istream & is, BigInteger & value) { /////////////////////////////////////////////////////
    if (is.good()) {
        char c = is.get();
        is.putback(c);
    }
    char value_cstr[BigInteger::MAX_LEN + 1];
    int curr_index = 0;
    if (is.peek() == '-') {
        value_cstr[curr_index] = is.get();
        ++curr_index;
    }
    for (; curr_index < BigInteger::MAX_LEN + 1 && !isspace(is.peek()); ++curr_index) {
        char curr_char = is.get();
        if (isdigit(curr_char)) {
            value_cstr[curr_index] = curr_char;
        }
        else {
            throw BigInteger::BigIntegerError();
        }
    }
    if (isdigit(is.peek())) {
        throw BigInteger::BigIntegerOverflow();
    }
    value_cstr[curr_index] = 0;
    value = value_cstr;
    return is;
}*/

BigInteger::operator bool() const {
    return len - 1 || digits[0];
}

BigInteger sqrt(const BigInteger & value) {             // dodn't have

    return value;
}

BigInteger abs(const BigInteger & value) {
    BigInteger tmp_value = value;
    tmp_value.minus = false;
    return tmp_value;
}
// 5/6 ready    (converting to int require upgrade)
//---------------------------------------------------------------------------