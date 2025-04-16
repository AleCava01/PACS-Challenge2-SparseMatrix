#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){

    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(5,5,0.7));
    std::vector<int> v = {1,1,1,1,1};

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

    // std::vector<int> row_0 = mat.extract_row(3);

    std::vector<int> result_moltiplication = mat.product_by_vector(v); // fuziona correttamente

    return 0;
}