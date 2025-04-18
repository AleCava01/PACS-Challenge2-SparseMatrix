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
    size_t n_row = 100;
    size_t n_col = 100;
    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(n_row,n_col,0.9));
    //Matrix<int, algebra::StorageOrder::RowMajor> mat(getTestMatrix());
    //std::vector<int> v = {1,2,1};
    std::vector<int> v = getRandomVector(n_row);

    verbose::separator(50);

    // Matrix compression

    // Compress + Decompress Now (test uncompressed multiplication)
    mat.compress();
    mat.decompress();  // force uncompressed mode (triggers else branch in product_by_vector)

    // Print compression status to verify logical branching
    std::cout << "[DEBUG] Is compressed? " << std::boolalpha << mat.is_compressed() << "\n";

    //mat.compress();
    //mat.decompress();
    mat.print();
    //mat.info();

    // Matrix * vector multiplication
    std::vector<int> multiplication_result = mat.product_by_vector(v);
    verbose::separator(50);
    std::cout << "v: ";
    print(v);
    auto [result, duration] = test_multiplication_mean(mat,v,100);
    verbose::display_mat_times_vector_results(result, duration);

    verbose::separator(50);

    /* // Decompression
    mat.decompress();
    //mat.printStorage();
    mat.print();
    //mat.info();

    // Matrix * vector multiplication
    multiplication_result = mat.product_by_vector(v);
    verbose::separator(50);
    std::cout << "v: ";
    print(v);
    std::tie(result, duration) = test_multiplication(mat,v);
    verbose::display_mat_times_vector_results(result, duration);
    verbose::separator(50); */



    return 0;
}