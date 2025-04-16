#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){

    // Sparse matrix Initialization
    size_t n_row = 5;
    size_t n_col = 6;
    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(n_row,n_col,0.7));

    verbose::separator(50);

    // Matrix compression
    mat.compress();
    mat.print();
    //mat.info();

    verbose::separator(50);

    // Decompression
    mat.decompress();
    mat.print();
    //mat.info();

    verbose::separator(50);

    // Matrix * vector multiplication
    std::vector<int> v = {1,0,0,0,2,1};
    std::vector<int> multiplication_result = mat.product_by_vector(v);
    print(multiplication_result);


    return 0;
}