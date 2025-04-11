#include "Matrix.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <typeinfo>

namespace algebra{

// METHODS IMPLEMENTATION OF MATRIX CLASS

// Matrix constructor
template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(size_t rows, size_t cols){
    rows_ = rows;
    cols_ = cols;
}

// only print the non-zero elements
template<typename T, StorageOrder Order>
void Matrix<T, Order>::print() const {
    for (std::size_t i = 0; i < rows_; ++i) {
        for (std::size_t j = 0; j < cols_; ++j) {
            std::array<int, 2> key = {static_cast<int>(i), static_cast<int>(j)};
            if (sparse_data_.count(key)) {
                std::cout << sparse_data_.at(key) << " ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
}

// Matrix info display function
template<typename T, StorageOrder Order>
void Matrix<T, Order>::info() const {
    std::cout << std::string(50, '*') << std::endl;
    std::cout << "*           Matrix Information Summary           *" << std::endl;
    std::cout << std::string(50, '*') << std::endl;
    std::cout << std::left;
    std::cout << std::setw(30) << "  Size:" << rows_ << " x " << cols_ << std::endl;
    std::cout << std::setw(30) << "  Storage Order:" << storageOrderToString(Order) << std::endl;
    std::cout << std::setw(30) << "  Element Type:" << demangle(typeid(T).name()) << std::endl;
    std::cout << std::string(50, '*') << std::endl;
}

// update(i,j,value) for sparse matrix
template<typename T, StorageOrder Order>
bool Matrix<T, Order>::update(const size_t i, const size_t j, const T& value) {
    std::array<int, 2> key = {static_cast<int>(i), static_cast<int>(j)};
    if (value != T(0)) {
        sparse_data_[key] = value;
    } else {
        sparse_data_.erase(key);
    }
    return true;
}

// ---------------------------------------------------------------------------------

// DEPRECATED
/* template<typename T, StorageOrder Order>
bool Matrix<T, Order>::update(const size_t k, const T& value){
    try {
        data_[k] = value;
        return true;
    } catch (const std::exception& e) { // catches possible problems in allocating the value, e.g. index out of bounds
        std::cerr << "Error while assigning value to matrix: " << e.what() << std::endl;
        return false;
    }
} */

// DEPRECATED
/* // Index calculation based on storage order (RowMajor or ColumnMajor)
template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::index(const size_t i, const size_t j) const {
    if constexpr (Order == StorageOrder::RowMajor) {
        // Row-major storage
        // (i,j) -> k
        // k = i*cols + j
        return i * cols_ + j;  
    } else {
        // Column-major storage
        // (i,j) -> k
        // k = j*rows + i
        return j * rows_ + i;
    }
}
 */




} // namespace algebra