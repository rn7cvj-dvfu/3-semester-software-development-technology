#include "ComplexNumber.h"

#ifndef COMPLEXNUMBERPACKER_H
#define COMPLEXNUMBERPACKER_H

class ComplexNumberPacker {
private:
    struct _complexDataStore {
        float real;
        float imaginary;
    };

    union _packStore {
        _complexDataStore complexNumberData;
        long long llRepresentation;
    };

public:
    ComplexNumber Unpack(long long packed);
    long long Pack(ComplexNumber complexNumber);
};

#endif