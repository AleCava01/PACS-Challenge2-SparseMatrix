#ifndef MATRIX_HPP
#define MATRIX_HPP

// STL Headers
#include <map>
#include <array>
#include <iostream>
#include <vector>
#include <iomanip>
#include <typeinfo>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

// External libraries
#include <omp.h> // for parallel computing (matrix * vector methods)
#include <zlib.h> // for reading .mtx.gz files (see Matrix Market Parsers)

// Project headers
#include "StorageOrder.hpp"
#include "CompressedMatrix.hpp"
#include "NormType.hpp"
#include "Utils.hpp"
#include "Parameters.hpp"

namespace algebra {

template<typename T, StorageOrder Order>
class Matrix {

private:
    // 📦 MATRIX DATA CONTAINERS
    size_t rows_, cols_;
    std::map<std::array<size_t, 2>, T> sparse_data_; // sparse dynamic storage: COOmap
    CompressedMatrix<T> compressed_data_; // compressed storage: CSR/CSC

    // 🔒 PRIVATE METHODS
    // Matrix Market Parser - Private Methods
    std::string mm_extract_gz(const std::string& filename);
    void mm_read_mtx();
    bool mm_stringstream_to_sparsedata_loader(const std::istringstream& iss);


public:
    // 🏗️ CONSTRUCTORS
    Matrix(size_t rows, size_t cols); // Constructor
    Matrix(const std::vector<std::vector<T>>& mat); // Constructor
    
    // 🔥 CORE METHODS
    bool update(const size_t i, const size_t j,const T& value); // Update element at position (i,j) to the specified value
    void compress(); //matrix compression (from COOmap to CSR/CSC)
    void decompress(); // matrix decompression (from CSR/CSC to COOmap)
    void resize(size_t new_rows, size_t new_cols);
    std::vector<T> compressed_product_by_vector(const std::vector<T>& v) const;
    std::vector<T> compressed_product_by_vector_parallel(const std::vector<T>& v) const;
    std::vector<T> product_by_vector(const std::vector<T>& v) const;
    std::vector<T> operator*(const Matrix<T, Order>& rhs) const;
    std::vector<T> diagonal_view() const;
    void transpose();
    template<NormType norm_type>
    T norm();

    // MATRIX MARKET PARSER + LOADER METHODS
    bool mm_load_mtx(const std::string& filename);

    // ℹ️ INFO & PRINTING METHODS
    bool is_compressed() const; // returns compression status
    size_t weight() const; // Returns the matrix space usage, in bytes
    void printStorage() const; // prints CSR/CSC vectors or COOmap mapping, depending on compression status
    void print(int width = 6) const; // Print matrix
    void info() const; // Prints some useful matrix info
    std::array<size_t, 2> size() const; // returns the size of the matrix as an array of {n_rows, n_cols}
};


} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP