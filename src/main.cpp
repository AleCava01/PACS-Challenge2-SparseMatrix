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

template<typename T, StorageOrder Order>
void multiply(const Matrix<T, Order>& mat, const std::vector<T>& v){
    std::vector<int> multiplication_result = mat.product_by_vector(v);
    verbose::separator(50);
    std::cout << "v: ";
    print(v);
    auto [result, duration] = test_multiplication_mean(mat,v,100);
    verbose::display_mat_times_vector_results(result, duration);
    verbose::separator(50);
}

int main(){
    // Sparse matrix and vector Initialization
    size_t n_row = 100;
    size_t n_col = 100;
    double sparsity_coeff = 0.9;
    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(n_row,n_col,sparsity_coeff));
    std::vector<int> v = getRandomVector(n_row);
    verbose::separator(50);

    // Matrix compression
    mat.compress();
    //mat.print();
    mat.info();
    multiply(mat, v); // test matrix * vector multiplication / compressed case

    // Matrix decompression
    mat.decompress();
    //mat.print();
    mat.info();
    multiply(mat, v); // test matrix * vector multiplication / uncompressed case

    return 0;
}