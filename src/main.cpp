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
    mat.printStorage();
    mat.print();
    //mat.info();

    // Matrix * vector multiplication
    std::vector<int> v = {1,0,0,0,2,1};
    std::vector<int> multiplication_result = mat.product_by_vector(v);

    verbose::separator(50);
    print(multiplication_result);
    verbose::separator(50);

    // Decompression
    mat.decompress();
    mat.printStorage();
    mat.print();
    //mat.info();

    // Matrix * vector multiplication
    multiplication_result = mat.product_by_vector(v);
    verbose::separator(50);
    print(multiplication_result);
    verbose::separator(50);



    return 0;
}