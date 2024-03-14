#include "ComplexNumberPacker.h"

template<is_numeric T>
ComplexNumber<T> ComplexNumberPacker<T>::Unpack(const char* packed)
{
	ComplexNumberPacker::_packStore* pack = new (packed) ComplexNumberPacker::_packStore;
	T real = pack->complexNumberData.real;
	T imaginary = pack->complexNumberData.imaginary;
    return *new ComplexNumber<T>(real, imaginary);
}

template<is_numeric T>
const char* ComplexNumberPacker<T>::Pack(ComplexNumber<T> complexNumber)
{
    ComplexNumberPacker::_complexDataStore complexDataStore;
	complexDataStore.real = complexNumber.GetReal();
	complexDataStore.imaginary = complexNumber.GetImaginary();
    ComplexNumberPacker::_packStore *packStore = new ComplexNumberPacker::_packStore;
    packStore->complexNumberData = complexDataStore;
    return (const char*)packStore;
}
