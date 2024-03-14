#include "ComplexNumber.h"

#ifndef COMPLEXNUMBERPACKER_H
#define COMPLEXNUMBERPACKER_H

template<is_numeric T>
class ComplexNumberPacker {
private:
    struct _complexDataStore {
        T real;
        T imaginary;
    };

    union _packStore {
        _complexDataStore complexNumberData;
        const char* repres;
    };

public:
    ComplexNumber<T> Unpack(const char* packed);
    const char* Pack(ComplexNumber<T> complexNumber);
};

#include "../src/ComplexNumberPacker.cpp"

#endif