#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

std::vector<std::vector<int>> getTestMatrix(){
    std::vector<std::vector<int>> matrix(3, std::vector<int>(3, 0)); // matrix initialization, all elements set at 0.
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][1] = 1;
    matrix[2][0] = 1;
    matrix[2][2] = 1;
    return matrix;
}

int main(){

    // Sparse matrix Initialization
    size_t n_row = 5;
    size_t n_col = 5;
    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(n_row,n_col,0.7));

    verbose::separator(50);

    // Matrix compression
    mat.compress();
    mat.printStorage();
    mat.print();
    //mat.info();

    // Matrix * vector multiplication
    std::vector<int> v = {1,0,1,0,0};
    std::vector<int> multiplication_result = mat.product_by_vector(v);
    verbose::separator(50);
    std::cout << "multiplication result: ";
    print(multiplication_result);
    verbose::separator(50);

    // Decompression
    mat.decompress();
    //mat.printStorage();
    mat.print();
    //mat.info();

    // Matrix * vector multiplication
    multiplication_result = mat.product_by_vector(v);
    verbose::separator(50);
    std::cout << "multiplication result: ";
    print(multiplication_result);
    verbose::separator(50);



    return 0;
}