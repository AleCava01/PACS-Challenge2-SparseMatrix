#include "../include/Matrix.hpp"
#include <iostream>

using namespace algebra;

int main(){
    Matrix<int, algebra::StorageOrder::RowMajor> mat(5, 8);
    mat.print();
    mat.update(1,2,4);
    
    std::cout<<std::endl;
    mat.print();

    std::cout<<std::endl;
    mat.update(3,3,6);
    mat.print();

    std::cout<<std::endl;
    mat.compress();
    mat.printCompressed();


    return 0;
}