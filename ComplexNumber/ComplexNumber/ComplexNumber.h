#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>

constexpr double EPS = 1e-8;

template<typename T>
struct ComplexNumber
{
public:
    ComplexNumber(T real, T imag);
    ComplexNumber(T real);
    ComplexNumber();

    T real() const;
    T imag() const;

    void set_real(T real);
    void set_imag(T imag);

    std::string to_string() const;
    T abs() const;

    ComplexNumber<T> pow(int exp) const;
    ComplexNumber<T> operator+(ComplexNumber<T> other) const;
    ComplexNumber<T> operator-(ComplexNumber<T> other) const;
    ComplexNumber<T> operator*(ComplexNumber<T> other) const;
    ComplexNumber<T> operator*(T num) const;
    ComplexNumber<T> operator/(ComplexNumber<T> other) const;
    ComplexNumber<T> operator/(T num) const;
    bool operator==(ComplexNumber<T> other) const;
    bool operator!=(ComplexNumber<T> other) const;

    bool eq(T num1, T num2) const;

private:
    T re, im;
};

template<typename T>
ComplexNumber<T>::ComplexNumber(T real, T imag) : re(real), im(imag) {}

template<typename T>
ComplexNumber<T>::ComplexNumber(T real) : re(real), im(0) {}

template<typename T>
ComplexNumber<T>::ComplexNumber() : re(0), im(0) {}

template<typename T>
T ComplexNumber<T>::real() const {
    return this->re;
}

template<typename T>
T ComplexNumber<T>::imag() const {
    return this->im;
}

template<typename T>
void ComplexNumber<T>::set_real(T real) {
    this->re = real;
}

template<typename T>
void ComplexNumber<T>::set_imag(T imag) {
    this->im = imag;
}

template<typename T>
std::string ComplexNumber<T>::to_string() const {
    return std::to_string(this->re) + "+" + std::to_string(this->im) + "i";
}

template<typename T>
T ComplexNumber<T>::abs() const {
    return std::sqrt(std::pow(this->re, 2) + std::pow(this->im, 2));
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::pow(int exp) const {
    T abs = this->abs();

    if (std::fabs(abs) < EPS)
        throw std::invalid_argument("division by zero");

    T expphi = std::acos(this->re / abs) * exp;

    T cos_expphi = std::cos(expphi);
    T sin_expphi = std::sin(expphi);

    return ComplexNumber(cos_expphi, sin_expphi) * std::pow(abs, exp);
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::operator+(ComplexNumber<T> other) const {
    return ComplexNumber(this->re + other.re, this->im + other.im);
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::operator-(ComplexNumber<T> other) const {
    return ComplexNumber(this->re - other.re, this->im - other.im);
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::operator*(ComplexNumber<T> other) const {
    return ComplexNumber(this->re * other.re - this->im * other.im, this->re * other.im + this->im * other.re);
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::operator*(T num) const {
    return ComplexNumber(this->re * num, this->im * num);
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::operator/(ComplexNumber<T> other) const {
    T div = std::pow(other.re, 2) + std::pow(other.im, 2);

    if (std::fabs(div) < EPS)
        throw std::invalid_argument("division by zero");

    return ComplexNumber((this->re * other.re + this->im * other.im), (this->im * other.re - this->re * other.im)) / div;
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::operator/(T num) const {
    if (std::fabs(num) < EPS)
        throw std::invalid_argument("division by zero");

    return ComplexNumber(this->re / num, this->im / num);
}

template<typename T>
bool ComplexNumber<T>::operator==(ComplexNumber<T> other) const {
    return eq(this->re, other.re) && eq(this->im, other.im);
}

template<typename T>
bool ComplexNumber<T>::operator!=(ComplexNumber<T> other) const {
    return !eq(this->re, other.re) || !eq(this->im, other.im);
}

template<typename T>
bool ComplexNumber<T>::eq(T num1, T num2) const {
    return std::fabs(num1 - num2) <= EPS;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ComplexNumber<T>& complex) {
    os << complex.to_string();
    return os;
}

#endif