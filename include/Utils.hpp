#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <chrono>
#include <tuple>
#include <cxxabi.h>
#include "../include/Matrix.hpp"


// General vector printer
template<typename T>
void print(const std::vector<T>& vec, const std::string& delimiter = ", "){
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size())
            std::cout << delimiter;
    }
    std::cout << "]" << std::endl;
}

namespace verbose{
    void separator(const size_t i){
        std::cout<<std::string(i,'-')<<std::endl;
    }
    template<typename T>
    void display_mat_times_vector_results(const std::vector<T>& multiplication_result, const auto& duration){
        std::cout << "Multiplication result: " << std::endl;
        print(multiplication_result);
        std::cout << "Execution time: " << duration << " µs" << std::endl;
    }

} // namespace verbose

using namespace algebra;

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

// Returns a random vector of integers ranging from 0 to 9 of size required.
std::vector<int> getRandomVector(const size_t n){
    srand(time(0));
    std::vector<int> vec(n, 0); // vector initialization, all elements set at 0.
    for (size_t i = 0; i < n; i++){
        vec[i]=1 + rand() % 9;
    }
    return vec;
}

// Overload operator * for vectors: computes the vector dot product.
template<typename T>
T operator*(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector sizes must match for dot product");
    }
    T result = T{};
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }
    return result;
}


#endif // UTILS_HPP