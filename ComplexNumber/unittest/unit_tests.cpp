#include <cassert>
#include <iostream>
#include "ComplexNumber.h"
#include "ComplexNumberPacker.h"

void test_ConstructorDefault() {
    ComplexNumber<float> c;
    assert(c.GetReal() == 0.0f);
    assert(c.GetImaginary() == 0.0f);
}

void test_Constructor() {
    ComplexNumber<int> c(10, 5);
    assert(c.GetReal() == 10);
    assert(c.GetImaginary() == 5);
}

void test_ConstructorPolar() {
    ComplexNumber<double> c(2, 3 * acos(-1)/4, true);
    assert(c.GetReal() + sqrt(2) < eps);
    assert(c.GetImaginary() - sqrt(2) < eps);
}

void test_FromFloat() {
    ComplexNumber<float> c1(5.1f, 0);
    ComplexNumber<float> c2(5.1f);

    assert(c1 == c2);
}

void test_GetReal() {
    ComplexNumber<float> c(23.123f, 31.12f);
    float expected = 23.123f;

    float actual = c.GetReal();

    assert(expected == actual);
}

void test_SetReal() {
    ComplexNumber<float> c(23.123f, 31.12f);
    float expected = 5.f;

    c.SetReal(5);

    assert(expected == c.GetReal());
}

void test_GetImaginary() {
    ComplexNumber<float> c(23.123f, 31.12f);
    float expected = 31.12f;

    float actual = c.GetImaginary();

    assert(expected == actual);
}

void test_SetImaginary() {
    ComplexNumber<float> c(23.123f, 31.12f);
    float expected = 5.f;

    c.SetReal(5);

    assert(expected == c.GetReal());
}

void test_Add() {
    ComplexNumber<float> expected(7.5f, 12.7f);
    ComplexNumber<float> c1(3, 8);
    ComplexNumber<float> c2(4.5f, 4.7f);

    auto actual = c1 + c2;

    assert((expected - actual).Abs() < eps);
}

void test_Sub() {
    ComplexNumber<float> expected(-1.5f, 3.3f);
    ComplexNumber<float> c1(3, 8);
    ComplexNumber<float> c2(4.5f, 4.7f);

    auto actual = c1 - c2;

    assert((expected - actual).Abs() < eps);
}

void test_Mult() {
    ComplexNumber<float> expected(-24.1f, 50.1f);
    ComplexNumber<float> c1(3, 8);
    ComplexNumber<float> c2(4.5f, 4.7f);

    auto actual = c1 * c2;

    assert((expected - actual).Abs() < eps);
}

void test_Div() {
    ComplexNumber<float> expected(1, -2);
    ComplexNumber<float> c1(7, -4);
    ComplexNumber<float> c2(3, 2);

    auto actual = c1 / c2;

    assert((expected - actual).Abs() < eps);
}

void test_Pow() {
    ComplexNumber<float> expected(8, acos(-1), true);
    double arg1 = ((2.f / 3.f) * acos(-1));
    ComplexNumber<float> c(4, arg1, true);
    float pow = 1.5f;

    auto actual = c ^ pow;
    
    assert((expected - actual).Abs() < eps);
}

void test_Abs() {
    ComplexNumber<float> a(-4.0, 3.0);
    auto res = a.Abs();
    assert(res - 5 < eps);
}

void test_Arg() {
    ComplexNumber<float> c(12, 4, true);
    assert(c.Arg() - 4 < eps);
}

void test_EqualComplex() {
    ComplexNumber<float> a(5, 10);
    ComplexNumber<float> b(5, 10);
    ComplexNumber<float> c(-5, -10);
    assert(a == b);
    assert(a != c);
}

void test_EqualsInt() {
    ComplexNumber<float> a(4, 0);
    int b = 4;
    assert(a == b);
    a.SetImaginary(2.f);
    assert(a != b);
}

void test_EqualsFloat() {
    ComplexNumber<float> a(10, 0);
    float b = 10;
    assert(a == b);
    a.SetImaginary(1);
    assert(a != b);
}

void test_Unpacking() {
    ComplexNumber<float> a(10, 2);
    ComplexNumberPacker<float> p;
    auto packed = p.Pack(a);
    ComplexNumber<float> b = p.Unpack(packed);
    assert(a.GetReal() == b.GetReal() && a.GetImaginary() == b.GetImaginary());
}

int main() {
    test_ConstructorDefault();
    test_Constructor();
    test_ConstructorPolar();
    test_FromFloat();
    test_GetReal();
    test_SetReal();
    test_GetImaginary();
    test_SetImaginary();
    test_Add();
    test_Sub();
    test_Mult();
    test_Div();
    test_Pow();
    test_Abs();
    test_Arg();
    test_EqualsInt();
    test_EqualsFloat();
    test_Unpacking();

    return 0;
}