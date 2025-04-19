#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main() {
   // Sparse matrix Initialization
    // 4x3 matrix with some non-zero elements
    std::vector<std::vector<int>> mat_data = {
        {1, 2, 3},
        {0, 1, 0},
        {4, 0, 1},
        {0, 0, 2}
    };

    // 3x1 vector (as a matrix)
    std::vector<std::vector<int>> vec_data = {
        {1},
        {2},
        {1}
    };

    Matrix<int, StorageOrder::RowMajor> mat(mat_data);
    Matrix<int, StorageOrder::RowMajor> vec_matrix(vec_data);

    // Compress the matrix
    mat.compress();

   
    std::vector<int> result = mat * vec_matrix;

   
    std::cout << "Matrix:\n";
    mat.decompress(); // Decompress to print the original matrix
    mat.print();

    std::cout << "Vector (as matrix):\n";
    vec_matrix.print();

    std::cout << "Result of mat * vec_matrix:\n";
    print(result);

    return 0;
}
