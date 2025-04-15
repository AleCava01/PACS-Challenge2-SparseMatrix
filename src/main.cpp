#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){

    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(5,5,0.7));

    verbose::separator(50);

    // Matrix compression
    mat.compress();
    mat.print();
    mat.info();

    verbose::separator(50);

    // Decompression
    mat.decompress();
    mat.print();
    mat.info();

    verbose::separator(50);

    return 0;
}