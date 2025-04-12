#include "Matrix.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <vector>

namespace algebra{

// METHODS IMPLEMENTATION OF MATRIX CLASS

// Matrix constructor
template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(size_t rows, size_t cols){
    rows_ = rows;
    cols_ = cols;
}

template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(const std::vector<std::vector<T>>& mat) {
    if (mat.empty() || mat[0].empty()) {
        rows_ = 0;
        cols_ = 0;
    } else {
        rows_ = mat.size();
        cols_ = mat[0].size();
        for (size_t i = 0; i < mat.size(); ++i) {
            for (size_t j = 0; j < mat[i].size(); ++j) {
                update(i, j, mat[i][j]);
            }
        }
    }
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

template<typename T, StorageOrder Order>
void Matrix<T, Order>::compress() {

    // Determine the conversion type
    constexpr bool isRowMajor = (Order == StorageOrder::RowMajor);
    size_t outer_size = isRowMajor ? rows_ : cols_;

    // Clears the values and preparation
    compressed_data_.values.clear();
    compressed_data_.inner_index.clear();
    compressed_data_.outer_ptr.assign(outer_size + 1, 0);  // initialise outer_ptr with zeroes

    // 1. Counts the values in each row/column
    for (const auto& [key, val] : sparse_data_) {
        int outer = isRowMajor ? key[0] : key[1];  // if CSR uses the row as outer if CSC uses the column  
        compressed_data_.outer_ptr[outer + 1]++;        // to know how many elements are in each column/row
    }                                              
    // 2. Indicates where starts each row
    for (size_t i = 1; i <= outer_size; ++i) {  // outer_ptr[i]   -> index of starting of the i-th row/column in the vector values  
                                                // outer_ptr[i+1] -> index excluded of the next row
        compressed_data_.outer_ptr[i] += compressed_data_.outer_ptr[i - 1];
    }

    // 3. Allocates space and to not call pushback each time 
    size_t nnz = sparse_data_.size();  // total number of non zero values 
    compressed_data_.values.resize(nnz);
    compressed_data_.inner_index.resize(nnz);
    std::vector<int> temp_offset = compressed_data_.outer_ptr;

    // 4. Giving values to the vectors
    for (const auto& [key, val] : sparse_data_) {
        int outer = isRowMajor ? key[0] : key[1];
        int inner = isRowMajor ? key[1] : key[0];
        int idx = temp_offset[outer]++;

        compressed_data_.values[idx] = val;
        compressed_data_.inner_index[idx] = inner;
    }
}

// Print Compressed Matrix
template<typename T, StorageOrder Order>
void Matrix<T, Order>::printCompressed() {

    std::cout << std::string(50, '-') << "\n";
    std::cout << "         Compressed Sparse Representation (CSR)\n";
    std::cout << std::string(50, '-') << "\n";

    std::cout << "Values:        ";
    for (const auto& v : compressed_data_.values) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "Inner index:   ";
    for (const auto& i : compressed_data_.inner_index) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "Outer pointer: ";
    for (const auto& o : compressed_data_.outer_ptr) {
        std::cout << o << " ";
    }
    std::cout << "\n";
    std::cout << std::string(50, '-') << "\n";
}


template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::weight_compressed() const{
    return sizeof(compressed_data_.values);
}
template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::weight_uncompressed() const{
    return sizeof(sparse_data_);
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