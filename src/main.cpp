#include "../include/Matrix.hpp"
#include <iostream>

using namespace algebra;

int main(){


    Matrix<int, algebra::StorageOrder::RowMajor> mat(1000, 1000);

    
    //for (int r : 1000) for (auto& x : r) if ((double)rand()/RAND_MAX > 0.95) x = rand() % 100 + 1;
    
    
    for (int i = 0; i < 1000; i++){
        for (int j = 0; j < 1000; j++){
            if ((double)rand()/RAND_MAX > 0.999){
                mat.update(i,j,1);
            }
        }
    }

    mat.print();
    mat.update(1,2,4);
    
    std::cout<<std::endl;
    mat.print();

    std::cout<<std::endl;
    mat.update(3,3,6);
    mat.print();

    std::cout<<std::endl;
    mat.compress();
    mat.printCompressed();
    std::cout << std::endl;
    std::cout << "Compressed matrix weight: " << mat.weight_compressed() << std::endl;
    std::cout << "Uncompressed matrix weight: " << mat.weight_uncompressed() << std::endl;



    return 0;
}