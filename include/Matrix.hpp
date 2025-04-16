#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "StorageOrder.hpp"
#include <map>
#include <array>
#include <iostream>
#include <vector>


namespace algebra {

// To store the 3 vectors for the CSR/CSM storage 
template<typename T>
struct CompressedMatrix {
    std::vector<T> values;        // Contains non zero values
    std::vector<size_t> inner_index; // Indexes of rows/columns
    std::vector<size_t> outer_ptr;   // Pointers to the starting row/column
};

template<typename T, StorageOrder Order>
class Matrix {

private:
    size_t rows_, cols_;
    std::map<std::array<size_t, 2>, T> sparse_data_; //sparse dynamic storage
    CompressedMatrix<T> compressed_data_; // compressed storage


public:
    Matrix(size_t rows, size_t cols); // Constructor
    Matrix(const std::vector<std::vector<T>>& mat); // Constructor

    void print() const; // Print matrix
    void info() const; // Prints some useful matrix info
    bool update(const size_t i, const size_t j,const T& value); // Update element at position (i,j) to the specified value
    bool is_compressed() const; // returns compression status
    void compress(); //matrix compression (from COOmap to CSR/CSC)
    void decompress(); // matrix decompression (from CSR/CSC to COOmap)
    void compressedInfo() const; // prints CSR/CSC vectors
    void printSparseData() const; 

    std::vector<T> product_by_vector(const std::vector<T>& v);
    std::vector<T> extract_row(const size_t index ,int& k);

    size_t weight() const;
    size_t find_row_for_index(size_t idx) const;


    //bool update(const size_t k, const T& value); // needed? to be decided.
};


} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP