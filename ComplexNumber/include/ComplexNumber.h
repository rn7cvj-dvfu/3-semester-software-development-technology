#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

#include <iostream>
#include <cmath>
#include <concepts>

#define eps 0.0001f

template<typename T>
	concept is_numeric = std::is_integral<T>::value || std::is_floating_point<T>::value;

template<is_numeric T>
class ComplexNumber {
private:
	T _real;
	T _imaginary;

public:
	ComplexNumber(T a, T b, bool polar_form = false);
	ComplexNumber();
	ComplexNumber(float a);

	T GetReal() const;
	T GetImaginary() const;

	void SetReal(T value);
	void SetImaginary(T value);

	ComplexNumber<T> Add(const ComplexNumber<T>& l, const ComplexNumber<T>& r);
	ComplexNumber<T> Sub(const ComplexNumber<T>& l, const ComplexNumber<T>& r);
	ComplexNumber<T> Mult(const ComplexNumber<T>& l, const ComplexNumber<T>& r);
	ComplexNumber<T> Div(const ComplexNumber<T>& l, const ComplexNumber<T>& r);
	ComplexNumber<T> Pow(const ComplexNumber<T>& num, float pow);

	float Abs() const;
	float Arg() const;

	ComplexNumber<T> operator+(const ComplexNumber<T>& r) const;
	ComplexNumber<T> operator-(const ComplexNumber<T>& r) const;
	ComplexNumber<T> operator*(const ComplexNumber<T>& r) const;
	ComplexNumber<T> operator/(const ComplexNumber<T>& r) const;
	ComplexNumber<T> operator^(float r) const;

	bool operator==(ComplexNumber<T>& r) const;
	bool operator==(T r) const;

	bool operator!=(ComplexNumber<T>& r) const;
	bool operator!=(T r) const;
};

template<is_numeric T>
std::ostream& operator << (std::ostream& ofstr, const ComplexNumber<T>& comp);

#include "../src/ComplexNumber.cpp"

#endif