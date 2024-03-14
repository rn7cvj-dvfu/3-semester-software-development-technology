#include <cassert>
#include <iostream>
#include "ComplexNumber.h"

using namespace std;  

int main() {

    ComplexNumber<double> c(1.2 , 2.2);

    cout << c << endl;

    return 0;
}