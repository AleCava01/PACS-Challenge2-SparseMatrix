#include "../include/Matrix.hpp"
#include <iostream>

using namespace algebra;

int main(){
    Matrix<int, algebra::StorageOrder::RowMajor> mat(5, 8);
    mat.print();
    mat.update(1,2,4);
    mat.update(12,6);

    std::cout<<std::endl;
    mat.print();
    return 0;
}