#ifndef _NickwideN_BigInteger_H
#define _NickwideN_BigInteger_H
#include <iostream>
#include <iomanip>  // setw, setfill
#include <cstring>  // strlen
class BigInteger {
private:
    const static int MAX_LEN = 20000;
    typedef int digit_t;
    typedef long long long_digit_t;
    const static digit_t BASE = 1000 * 1000 * 1000;
    const static int BASE_LOG10 = 9;
    const static int MAX_LEN_BASE = MAX_LEN / BASE_LOG10 + (MAX_LEN % BASE_LOG10 ? 1 : 0);
    const static int default_value = 0;
    int len;    //len_BASE
    bool minus;
    digit_t digits[MAX_LEN_BASE];

    //--------------------------------------------------------------------
    //      Privite friend functions:    (functions take left_value and right_value only as pozitive)
    friend BigInteger & Addition(BigInteger & left_value, const BigInteger & right_value);  // similar operator += (left_value += right_value)
    friend BigInteger & Substraction(BigInteger & left_value, const BigInteger & right_value);  // similar operator -= (left_value -= right_value)
    friend BigInteger Multiplication(const BigInteger & left_value, const BigInteger & right_value);  // similar operator *= (left_value *= right_value)
    friend bool Is_abs_left_value_Fewer_abs_right_value(const BigInteger & left_value, const BigInteger & right_value);
    //      functions for Division:
    friend BigInteger & prev_for_div(BigInteger ** right_mul_bin_pnt, const int & i, const int & j, const int & right_mul_bin_cap);
    friend BigInteger ** binary_search_div_with_create(BigInteger ** right_mul_bin_pnt, int & right_mul_bin_pnt_cap, const int & right_mul_bin_cap,
        int & right_mul_bin_pnt_num, int & right_mul_bin_num, const BigInteger & left_value, const BigInteger & right_value, BigInteger & result);
    friend void binary_search_div_without_create(BigInteger ** right_mul_bin_pnt, int & right_mul_bin_pnt_cap, const int & right_mul_bin_cap,
        int & right_mul_bin_pnt_num, int & right_mul_bin_num, const BigInteger & left_value, const BigInteger & right_value, BigInteger & result);
    friend BigInteger ** binary_search_div(BigInteger ** right_mul_bin_pnt, int & right_mul_bin_pnt_cap, const int & right_mul_bin_cap,
        int & right_mul_bin_pnt_num, int & right_mul_bin_num, const BigInteger & left_value, const BigInteger & right_value, BigInteger & result);
    //     method for constructors that use integer values
    template <typename user_t>
    BigInteger * define_this(BigInteger * _this, const user_t & value);
public:
    //--------------------------------------------------------------------
    class BigIntegerError{};
    class BigIntegerDivisionByZero : public BigIntegerError {};
    class BigIntegerOverflow : public BigIntegerError {};
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    //      Constructors:
    BigInteger();
    BigInteger(const int & value);
    BigInteger(const unsigned int & value);
    BigInteger(const long long & value);
    BigInteger(const unsigned long long & value);
    BigInteger(const char * value_str);
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    //      Operators:
    friend BigInteger operator + (const BigInteger & left_value_value, const BigInteger & right_value_value);
    BigInteger & operator += (const BigInteger & right_value);
    friend BigInteger operator - (const BigInteger & left_value, const BigInteger & right_value);
    BigInteger & operator -= (const BigInteger & right_value);
    friend BigInteger operator * (const BigInteger & left_value, const BigInteger & right_value);
    BigInteger & operator *= (const BigInteger & right_value);
    friend BigInteger operator % (const BigInteger & left_value, const BigInteger & right_value); 
    BigInteger & operator %= (const BigInteger & right_value);
    friend BigInteger operator / (const BigInteger & left_value, const BigInteger & right_value); 
    BigInteger & operator /= (const BigInteger & right_value);                    
    friend BigInteger operator ^ (const BigInteger & value, const int & exponent);
    BigInteger & operator ^= (const int & exponent);
    //16
    BigInteger operator - () const;
    BigInteger operator + () const;
    BigInteger & operator ++ ();
    BigInteger & operator -- ();
    BigInteger operator ++ (int);
    BigInteger operator -- (int);
    //6
    
    friend bool operator < (const BigInteger & left_value, const BigInteger & right_value);
    friend bool operator == (const BigInteger & left_value, const BigInteger & right_value);
    friend bool operator > (const BigInteger & left_value, const BigInteger & right_value);
    friend bool operator <= (const BigInteger & left_value, const BigInteger & right_value);
    friend bool operator >= (const BigInteger & left_value, const BigInteger & right_value);
    friend bool operator != (const BigInteger & left_value, const BigInteger & right_value);
    //6
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    //      Other functions:
    friend std::ostream & operator << (std::ostream & os, const BigInteger & number);
    friend std::istream & operator >> (std::istream & is, BigInteger & number);
    explicit operator bool() const;     
    friend BigInteger sqrt(const BigInteger & number);                         // dodn't have
    friend BigInteger abs(const BigInteger & number);
    //6
    //--------------------------------------------------------------------

};


#endif // !_NickwideN_BigInteger_H

template<typename user_t>
inline BigInteger * BigInteger::define_this(BigInteger * _this, const user_t & value) {
    user_t tmp_value = value;
    int curr_index_digits = 0;
    for (; curr_index_digits < MAX_LEN_BASE && tmp_value; ++curr_index_digits) {
        _this->digits[curr_index_digits] = tmp_value % BASE;
        tmp_value /= BASE;
    }
    if (tmp_value) {
        throw BigIntegerOverflow();
    }
    _this->len = curr_index_digits;
    if (!_this->len) {
        ++_this->len;
    }
    for (; curr_index_digits < MAX_LEN_BASE; ++curr_index_digits) {
        _this->digits[curr_index_digits] = default_value;
    }
    _this->minus = false;
    return _this;
}