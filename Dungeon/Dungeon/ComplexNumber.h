#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

#include <iostream>
#include <cmath>

#define eps 0.0001f

class ComplexNumber {
private:
	float _real;
	float _imaginary;

public:
	ComplexNumber();
	ComplexNumber(float a, float b, bool polar_form = false);
	ComplexNumber(float a);
	ComplexNumber(int a);

	float GetReal() const;
	float GetImaginary() const;
	void SetReal(float value);
	void SetImaginary(float value); 

	float Abs() const;
	float Arg() const;

	static ComplexNumber Add(const ComplexNumber& l, const ComplexNumber& r);
	static ComplexNumber Sub(const ComplexNumber& l, const ComplexNumber& r);
	static ComplexNumber Mult(const ComplexNumber& l, const ComplexNumber& r);
	static ComplexNumber Div(const ComplexNumber& l, const ComplexNumber& r);
	static ComplexNumber Pow(const ComplexNumber& num, float pow);

	ComplexNumber operator + (const ComplexNumber& r) const;
	ComplexNumber operator - (const ComplexNumber& r) const;
	ComplexNumber operator * (const ComplexNumber& r) const;
	ComplexNumber operator / (const ComplexNumber& r) const;
	ComplexNumber operator ^ (float r) const;

	bool operator == (ComplexNumber& r) const;
	bool operator == (float r) const;
	bool operator == (int r) const;
	bool operator != (ComplexNumber& r) const;
	bool operator != (float r) const;
	bool operator != (int r) const;
};

std::ostream& operator << (std::ostream& ofstr, const ComplexNumber& comp);

#endif