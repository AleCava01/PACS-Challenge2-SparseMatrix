#include "Matrix.hpp"
#include <iostream>

namespace algebra{

// METHODS IMPLEMENTATION OF MATRIX CLASS

// Matrix constructor
template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(size_t rows, size_t cols){
    rows_ = rows;
    cols_ = cols;
    data_.resize(rows * cols);
}

// basic Matrix print function
template<typename T, StorageOrder Order>
void Matrix<T, Order>::print() const {
    for (std::size_t i = 0; i < rows_; ++i) {
        for (std::size_t j = 0; j < cols_; ++j) {
            std::cout << data_[index(i,j)] << " ";
        }
        std::cout << std::endl;
    }
}

// Index calculation based on storage order (RowMajor or ColumnMajor)
template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::index(const size_t i, const size_t j) const {
    if constexpr (Order == StorageOrder::RowMajor) {
        return i * cols_ + j;  // Row-major storage
    } else {
        return j * rows_ + i;  // Column-major storage
    }
}


} // namespace algebra