#include "ComplexNumberPacker.h"

ComplexNumber ComplexNumberPacker::Unpack(long long packed)
{
	ComplexNumberPacker::_packStore pack;
	pack.llRepresentation = packed;
	float real = pack.complexNumberData.real;
	float imaginary = pack.complexNumberData.imaginary;
    return *new ComplexNumber(real, imaginary);
}

long long ComplexNumberPacker::Pack(ComplexNumber complexNumber)
{
    ComplexNumberPacker::_complexDataStore complexDataStore;
	complexDataStore.real = complexNumber.GetReal();
	complexDataStore.imaginary = complexNumber.GetImaginary();
    ComplexNumberPacker::_packStore packStore;
    packStore.complexNumberData = complexDataStore;
    return packStore.llRepresentation;
}
