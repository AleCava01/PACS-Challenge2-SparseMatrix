#include "../include/Matrix.hpp"
#include <iostream>

using namespace algebra;

int main(){
    Matrix<int, algebra::StorageOrder::RowMajor> mat(5, 8);
    mat.print();
    return 0;
}