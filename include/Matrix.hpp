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

/**
 * @brief A sparse matrix class with optional compression.
 * 
 * @tparam T Type of the matrix elements.
 * @tparam Order Storage order (RowMajor or ColumnMajor).
 * 
 * Provides efficient storage and operations for sparse matrices,
 * supporting both COO and compressed (CSR/CSC) formats.
 * Includes functionalities like update, compression, decompression,
 * resizing, and matrix-vector multiplication (serial and parallel).
 */
template<typename T, StorageOrder Order>
class Matrix {

private:
    // 📦 MATRIX DATA CONTAINERS

    size_t rows_; ///< Number of rows.
    size_t cols_; ///< Number of columns.

    std::map<std::array<size_t, 2>, T> sparse_data_; ///< Sparse dynamic storage: COOmap format.
    CompressedMatrix<T> compressed_data_; ///< Compressed storage: CSR/CSC format.

    // 🔒 PRIVATE METHODS

    /**
     * @brief Extracts the content of a gzipped Matrix Market file.
     * 
     * Uses zlib's gz functions to read the file in chunks and accumulate its contents
     * 
     * @param filename Path to the .mtx.gz file.
     * @return Extracted content as a string.
     */
    std::string mm_extract_gz(const std::string& filename);

    /**
     * @brief Reads a Matrix Market file into the sparse data structure.
     */
    void mm_read_mtx();

    /**
     * @brief Parses a Matrix Market file's content (from a stringstream) into sparse matrix data format.
     * 
     * Reads the header to get matrix dimensions, then processes triplets (row, column, value) to populate the matrix.
     * 
     * 
     * @param iss_original an input stringstream containing the file content
     * @return True if parsing was successful, false otherwise.
     */
    bool mm_stringstream_to_sparsedata_loader(const std::istringstream& iss_original);

public:
    // 🏗️ CONSTRUCTORS

    /**
     * @brief Constructor initializing an empty matrix (all values are set to 0) with specified dimensions.
     * 
     * @param rows Number of rows.
     * @param cols Number of columns.
     */
    Matrix(size_t rows, size_t cols);

    /**
     * @brief Constructor initializing a matrix from a 2D vector (std::vector<T>).
     * 
     * Initializes the matrix dimensions and populates sparse_data_ using the update() method.
     * 
     * @param mat 2D vector representing the matrix.
     */
    Matrix(const std::vector<std::vector<T>>& mat);

    // 🔥 CORE METHODS

    /**
     * @brief Updates an element at a specific position.
     * 
     * Updates the value at position (i, j) in the uncompressed (sparse_data_) format.
     * Inserts or updates the value if it's non-zero; removes the entry if the value is zero.
     * 
     * @param i Row index.
     * @param j Column index.
     * @param value New value to set.
     * @return True if update was successful, false otherwise.
     */
    bool update(const size_t i, const size_t j, const T& value);

    /**
     * @brief Compresses the matrix from sparse to compressed format.
     * 
     * * Converts the matrix from uncompressed (Coo-MAP) format to compressed format (CSR for RowMajor, CSC for ColMajor).
     * It builds compressed_data_ from sparse_data_ and then clears the uncompressed storage to save memory.
     */
    void compress();

    /**
     * @brief Decompresses the matrix from compressed to sparse format.
     * 
     * Converts the matrix from compressed form (CSR/CSC) to uncompressed form (Coo-MAP).
     * It reconstructs sparse_data_ using the compressed_data_ arrays and clears the compressed storage afterward.
     * 
     */
    void decompress();

    /**
     * @brief Resizes the matrix to new dimensions.
     * 
     * Removes all elements outside the new bounds.
     * 
     * @param new_rows New number of rows.
     * @param new_cols New number of columns.
     */
    void resize(size_t new_rows, size_t new_cols);

    /**
     * @brief Multiplies the compressed matrix by a vector.
     * 
     * @param v Input vector.
     * @return Resulting vector after multiplication.
     */
    std::vector<T> compressed_product_by_vector(const std::vector<T>& v) const;

    /**
     * @brief Multiplies the compressed matrix by a vector in parallel.
     * 
     * Uses OpenMP to parallelize the dot product calculation for each row of the matrix.
     * 
     * @param v Input vector.
     * @return Resulting vector after multiplication.
     */
    std::vector<T> compressed_product_by_vector_parallel(const std::vector<T>& v) const;

    /**
     * @brief Multiplies the (possibly uncompressed) matrix by a vector.
     * 
     * Multiplies the matrix by a vector v. If the matrix is compressed, it uses either parallel or regular multiplication based on the number of rows (CSR case) or columns (CSC case).
     * For uncompressed matrices in COO format, it performs the multiplication by iterating over sparse data.
     * Inputs: v - a vector of type T, Outputs: a vector of type T with the result of the multiplication.
     * 
     * @param v Input vector.
     * @return Resulting vector after multiplication.
     */
    std::vector<T> product_by_vector(const std::vector<T>& v) const;

    /**
     * @brief Overloads the * operator to perform matrix-vector multiplication when rhs is a column vector matrix.
     * 
     * Assumes rhs is effectively a column vector; converts it to std::vector<T> and uses product_by_vector for the computation.
     * 
     * @param rhs Right-hand side matrix.
     * @return Resulting vector after multiplication.
     */
    std::vector<T> operator*(const Matrix<T, Order>& rhs) const;

    /**
     * @brief Extracts the diagonal of the matrix.
     * 
     * @return Vector containing diagonal elements.
     */
    std::vector<T> diagonal_view() const;

    /**
     * @brief Transposes the matrix (rows become columns and vice versa).
     * 
     * Transposes the matrix in-place by swapping rows and columns.
     * Updates the sparse data (non-zero entries) by flipping their indices.
     * Recompresses the matrix if it was previously compressed before transposing.
     * 
     */
    void transpose();

    /**
     * @brief Computes a matrix norm.
     * 
     * Computes the matrix norm (One, Infinity, or Frobenius) depending on the template parameter.
     * Handles both compressed (CSR/CSC) and uncompressed (COOmap) storage formats, with automatic transposition if needed.
     * Returns a scalar value representing the computed norm.
     * 
     * @tparam norm_type Type of norm to compute.
     * @return Computed norm value.
     */
    template<NormType norm_type>
    T norm();

    // MATRIX MARKET PARSER + LOADER METHODS

    /**
     * @brief Loads a Matrix Market (.mtx or .mtx.gz) file and parses its contents into sparse data format.
     * 
     * Supports both compressed (.mtx.gz) and uncompressed (.mtx) Matrix Market files.
     * 
     * @param filename Path to the file.
     * @return True if loading was successful, false otherwise.
     */
    bool mm_load_mtx(const std::string& filename);

    // ℹ️ INFO & PRINTING METHODS

    /**
     * @brief Checks if the matrix is in compressed form.
     * 
     * @return True if compressed, false otherwise.
     */
    bool is_compressed() const;

    /**
     * @brief Calculates the memory usage of the matrix in bytes.
     * 
     * Calculates the memory usage (weight) of the matrix based on its storage format.
     * For compressed matrices, it sums the sizes of the values, indices, and pointers; for uncompressed, it estimates based on the sparse data structure.
     * 
     * @return Memory usage in bytes.
     */
    size_t weight() const;

    /**
     * @brief Prints the internal storage format (COOmap or CSR/CSC).
     * 
     * Displays the compressed sparse representation (CSR/CSC) or the uncompressed COO format, showing values, indices, and pointers.
     * 
     */
    void printStorage() const;

    /**
     * @brief Prints the matrix contents.
     * 
     * @param width Field width for each printed element (default is 6).
     */
    void print(int width = 6) const;

    /**
     * @brief Prints information about the matrix (size, storage type, etc.).
     */
    void info() const;

    /**
     * @brief Returns the dimensions of the matrix.
     * 
     * @return An array {rows, cols}.
     */
    std::array<size_t, 2> size() const;
};

} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP
