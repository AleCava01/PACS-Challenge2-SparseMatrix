#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <cxxabi.h>


namespace verbose{
    void separator(const size_t i){
        std::cout<<std::string(i,'-')<<std::endl;
    }
} // namespace verbose

// Function to get the type name from
std::string demangle(const char* mangled) {
    int status = 0;
    char* realname = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
    std::string result = (status == 0 && realname) ? realname : mangled;
    free(realname);
    return result;
}

// Returns a random matrix of integers ranging from 0 to 9 of size required. random factor is used to set the rate of zeros in the sparse matrix.
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

#endif // UTILS_HPP