#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include "StorageOrder.hpp"
#include <map>
#include <array>
#include <iostream>


namespace algebra {

// To store the 3 vectors for the CSR/CSM storage 
template<typename T>
struct CompressedMatrix {
    std::vector<T> values;        // Contains non zero values
    std::vector<int> inner_index; // Indexes of rows/columns
    std::vector<int> outer_ptr;   // Pointers to the starting row/column
};

template<typename T, StorageOrder Order>
class Matrix {

private:
    size_t rows_, cols_;
    std::map<std::array<int, 2>, T> sparse_data_; //sparse dynamic storage
    CompressedMatrix<T> compressed_data_; // compressed storage
      

public:
    Matrix(size_t rows, size_t cols); // Constructor
    void print() const; // Print matrix
    void info() const; // Prints some useful matrix info
    bool update(const size_t i, const size_t j,const T& value); // Update element at position (i,j) to the specified value
    bool update(const std::vector<std::vector<T>>& mat);
    void compress();
    void printCompressed();
    size_t weight_compressed() const;
    size_t weight_uncompressed() const;

    //bool update(const size_t k, const T& value); // needed? to be decided.
};


} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP