#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <map>
#include <array>
#include <iostream>
#include <vector>
#include "StorageOrder.hpp"
#include "CompressedMatrix.hpp"

namespace algebra {

template<typename T, StorageOrder Order>
class Matrix {

private:
    size_t rows_, cols_;
    std::map<std::array<size_t, 2>, T> sparse_data_; // sparse dynamic storage: COOmap
    CompressedMatrix<T> compressed_data_; // compressed storage: CSR/CSC

public:
    // CONSTRUCTORS
    Matrix(size_t rows, size_t cols); // Constructor
    Matrix(const std::vector<std::vector<T>>& mat); // Constructor
    
    // CORE METHODS
    bool update(const size_t i, const size_t j,const T& value); // Update element at position (i,j) to the specified value
    void compress(); //matrix compression (from COOmap to CSR/CSC)
    void decompress(); // matrix decompression (from CSR/CSC to COOmap)
    std::vector<T> product_by_vector(const std::vector<T>& v) const;
    std::vector<T> extract_row(const size_t index, size_t k) const;

    // INFO & PRINTING METHODS
    bool is_compressed() const; // returns compression status
    size_t weight() const; // Returns the matrix space usage, in bytes
    void printStorage() const; // prints CSR/CSC vectors or COOmap mapping, depending on compression status
    void print() const; // Print matrix
    void info() const; // Prints some useful matrix info
};


} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP