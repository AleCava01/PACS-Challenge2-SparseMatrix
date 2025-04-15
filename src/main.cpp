#include "../include/Matrix.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

std::vector<std::vector<int>> getRandomSparseMatrix(const size_t rows, const size_t cols, const double random_factor){
    srand(time(0));
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0)); // matrix initialization, all elements set at 0.
    
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if ((double)rand()/RAND_MAX > random_factor){ 
                matrix[i][j]=1 + rand() % 9;
            }
        }
    }
    return matrix;
}

int main(){

    Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix(5,5,0.7));
    mat.print();

    // Compressione della matrice
    mat.compress();
    std::cout << (mat.is_compressed() ? "The matrix is compressed" : "The matrix is not compressed") << std::endl;

    // Print dei vettori di compressione
    mat.compressedInfo();
    mat.print();

    // Uncompression
    mat.uncompress();
    if(mat.is_compressed()){
        std::cout << "The matrix is compress" << std::endl;
    }
    else{
        std::cout << "The matrix is not compress" << std::endl;
    }

/* 

    std::cout << std::endl;
    std::cout << "Compressed matrix weight: " << mat.weight_compressed() << std::endl;
    std::cout << "Uncompressed matrix weight: " << mat.weight_uncompressed() << std::endl;


    Matrix<int, algebra::StorageOrder::RowMajor> mat2(5,5);
    std::cout << "Compressed matrix weight: " << mat2.weight_compressed() << std::endl;
    std::cout << "Uncompressed matrix weight: " << mat2.weight_uncompressed() << std::endl;

 */


    return 0;
}