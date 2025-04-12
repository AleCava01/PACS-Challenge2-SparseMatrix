#include "../include/Matrix.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){

    //for (int r : 1000) for (auto& x : r) if ((double)rand()/RAND_MAX > 0.95) x = rand() % 100 + 1;
    std::vector<std::vector<int>> matrix(1000, std::vector<int>(1000, 0));  // 1000x1000 matrix, inizializzata a 0
    
    for (int i = 0; i < 1000; i++){
        for (int j = 0; j < 1000; j++){
            if ((double)rand()/RAND_MAX > 0.999){
                matrix[i][j]=1;
            }
        }
    }
    Matrix<int, algebra::StorageOrder::RowMajor> mat(matrix);

    mat.compress();

    std::cout << std::endl;
    std::cout << "Compressed matrix weight: " << mat.weight_compressed() << std::endl;
    std::cout << "Uncompressed matrix weight: " << mat.weight_uncompressed() << std::endl;


    Matrix<int, algebra::StorageOrder::RowMajor> mat2(5,5);
    std::cout << "Compressed matrix weight: " << mat2.weight_compressed() << std::endl;
    std::cout << "Uncompressed matrix weight: " << mat2.weight_uncompressed() << std::endl;




    return 0;
}