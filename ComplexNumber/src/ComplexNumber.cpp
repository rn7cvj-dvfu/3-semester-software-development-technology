#include "ComplexNumber.h"

template<is_numeric T>
ComplexNumber<T>::ComplexNumber(T a, T b, bool polar_form)
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

template<is_numeric T>
ComplexNumber<T>::ComplexNumber() : ComplexNumber(0, 0) {}

template<is_numeric T>
ComplexNumber<T>::ComplexNumber(float a) : ComplexNumber<T>(a, 0) {}

template<is_numeric T>
T ComplexNumber<T>::GetReal() const { return _real; }

template<is_numeric T>
T ComplexNumber<T>::GetImaginary() const { return _imaginary; }

template<is_numeric T>
void ComplexNumber<T>::SetReal(T value) { _real = value; }

template<is_numeric T>
void ComplexNumber<T>::SetImaginary(T value) { _imaginary = value; }

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::Add(const ComplexNumber<T>& l, const ComplexNumber<T>& r)
{
	return ComplexNumber<T>(l._real + r._real, l._imaginary + r._imaginary);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::Sub(const ComplexNumber<T>& l, const ComplexNumber<T>& r)
{
	return ComplexNumber<T>(l._real - r._real, l._imaginary - r._imaginary);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::Mult(const ComplexNumber<T>& l, const ComplexNumber<T>& r)
{
	return ComplexNumber<T>(l.Abs() * r.Abs(), l.Arg() + r.Arg(), true);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::Div(const ComplexNumber<T>& l, const ComplexNumber<T>& r)
{
	T r_abs = r.Abs();
	if (r_abs == 0) throw "Zero Division";
	return ComplexNumber<T>(l.Abs()/r_abs, l.Arg() - r.Arg(), true);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::Pow(const ComplexNumber<T>& num, float pow)
{
	return ComplexNumber<T>(std::pow(num.Abs(), pow), num.Arg() * pow, true);
}

template<is_numeric T>
float ComplexNumber<T>::Abs() const
{
	return sqrt(_real * _real + _imaginary * _imaginary);
}

template<is_numeric T>
float ComplexNumber<T>::Arg() const
{
	if (!_real) return acos(-1);
	if (_real > 0) return atan(_imaginary / _real);
	return acos(-1) + atan(_imaginary / _real);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::operator+(const ComplexNumber<T>& r) const
{
	return Add(const_cast<const ComplexNumber<T>&>(*this), r);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::operator-(const ComplexNumber<T>& r) const
{
	return Sub(const_cast<const ComplexNumber<T>&>(*this), r);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::operator*(const ComplexNumber<T>& r) const
{
	return Mult(const_cast<const ComplexNumber<T>&>(*this), r);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::operator/(const ComplexNumber<T>& r) const
{
	return Div(const_cast<const ComplexNumber<T>&>(*this), r);
}

template<is_numeric T>
ComplexNumber<T> ComplexNumber<T>::operator^(float r) const
{
	return Pow(const_cast<const ComplexNumber<T>&>(*this), r);
}

template<is_numeric T>
bool ComplexNumber<T>::operator==(ComplexNumber<T>& r) const
{
	return ((_real - r._real < eps) && (_imaginary - r._imaginary < eps));
}

template<is_numeric T>
bool ComplexNumber<T>::operator==(T r) const
{
	return ((_imaginary < eps) && (_real - r < eps));
}

template<is_numeric T>
bool ComplexNumber<T>::operator!=(ComplexNumber<T>& r) const
{
	return !(*this == r);
}

template<is_numeric T>
bool ComplexNumber<T>::operator!=(T r) const
{
	return !(*this == r);
}

template<is_numeric T>
std::ostream& operator<< (std::ostream& ofstr, const ComplexNumber<T>& comp)
{
	return ofstr << comp.GetReal() << " + i*" << comp.GetImaginary();
}