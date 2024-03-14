#include "ComplexNumber.h"

ComplexNumber::ComplexNumber(float a, float b, bool polar_form = false)
{
	if (polar_form)
	{
		_real = a * cos(b);
		_imaginary = a * sin(b);
	}
	else
	{
		_real = a;
		_imaginary = b;
	}
}

ComplexNumber::ComplexNumber() : ComplexNumber(0, 0) {}

ComplexNumber::ComplexNumber(float a) : ComplexNumber(a, 0) {}

float ComplexNumber::GetReal() const { return _real; }

float ComplexNumber::GetImaginary() const { return _imaginary; }

void ComplexNumber::SetReal(float value) { _real = value; }

void ComplexNumber::SetImaginary(float value) { _imaginary = value; }

ComplexNumber ComplexNumber::Add(const ComplexNumber& l, const ComplexNumber& r)
{
	return ComplexNumber(l._real + r._real, l._imaginary + r._imaginary);
}

ComplexNumber ComplexNumber::Sub(const ComplexNumber& l, const ComplexNumber& r)
{
	return ComplexNumber(l._real - r._imaginary, l._real - r._imaginary);
}

ComplexNumber ComplexNumber::Mult(const ComplexNumber& l, const ComplexNumber& r)
{
	return ComplexNumber(l.Abs() * r.Abs(), l.Arg() + r.Arg(), true);
}

ComplexNumber ComplexNumber::Div(const ComplexNumber& l, const ComplexNumber& r)
{
	if (r.Abs() == 0) throw "Zero Division";
	return ComplexNumber(l.Abs()/r.Abs(), l.Arg() - r.Arg(), true);
}

ComplexNumber ComplexNumber::Pow(const ComplexNumber& num, float pow)
{
	return ComplexNumber(std::pow(num.Abs(), pow), num.Arg() * pow, true);
}

float ComplexNumber::Abs() const
{
	return sqrt(_real * _real + _imaginary * _imaginary);
}

float ComplexNumber::Arg() const
{
	if (!_real) return acos(-1);
	if (_real > 0) return atan(_imaginary / _real);
	return acos(-1) + atan(_imaginary / _real);
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& r) const
{
	return Add(*this, r);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& r) const
{
	return Sub(*this, r);
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber& r) const
{
	return Mult(*this, r);
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber& r) const
{
	return Div(*this, r);
}

ComplexNumber ComplexNumber::operator^(float r) const
{
	return Pow(*this, r);
}

bool ComplexNumber::operator==(ComplexNumber& r) const
{
	return ((_real - r._real < eps) && (_imaginary - r._imaginary < eps));
}

bool ComplexNumber::operator==(float r) const
{
	return ((_imaginary < eps) && (_imaginary - r < eps));
}

bool ComplexNumber::operator==(int r) const
{
	return ((_imaginary < eps) && (_imaginary - r) < eps);
}

bool ComplexNumber::operator!=(ComplexNumber& r) const
{
	return !(*this == r);
}

bool ComplexNumber::operator!=(float r) const
{
	return !(*this == r);
}

bool ComplexNumber::operator!=(int r) const
{
	return !(*this == r);
}

std::ostream& operator << (std::ostream& ofstr, const ComplexNumber& comp)
{
	return ofstr << comp.GetReal() << " + i*" << comp.GetImaginary();
}