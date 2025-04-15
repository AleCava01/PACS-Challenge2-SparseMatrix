#include "../include/Matrix.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){
    srand(time(0));     // set seed based on current time

    //for (int r : 1000) for (auto& x : r) if ((double)rand()/RAND_MAX > 0.95) x = rand() % 100 + 1;
    std::vector<std::vector<int>> matrix(5, std::vector<int>(5, 0));  // 1000x1000 matrix, inizializzata a 0
    
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            if ((double)rand()/RAND_MAX > 0.7){
                matrix[i][j]=1 + rand() % 9;
            }
        }
    }

    Matrix<int, algebra::StorageOrder::RowMajor> mat(matrix);

/*     // Print iniziale
    mat.printSparseData();
    mat.print(); */

    // Compressione della matrice
    mat.compress();
    if(mat.is_compressed()){
        std::cout << "The matrix is compress" << std::endl;
    }
    else{
        std::cout << "The matrix is not compress" << std::endl;
    }

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