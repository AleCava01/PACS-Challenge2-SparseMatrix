/* This .tpp file contains the implementation of the Matrix class template. 
It defines methods for matrix operations, including sparse matrix compression, matrix-vector multiplication, and I/O operations. 
The implementation supports both uncompressed (COO) and compressed (CSR/CSC) formats, optimizing memory usage and performance for large datasets.
 */

#include "Matrix.hpp"
#include "Utils.hpp"
#include "Parameters.hpp"

#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <omp.h>
#include <zlib.h>
#include <sstream>
#include <string>

namespace algebra{

// 🏗️ CONSTRUCTORS
template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(size_t rows, size_t cols){
    rows_ = rows;
    cols_ = cols;
}

template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(const std::vector<std::vector<T>>& mat) {
// Constructor from a 2D vector.
// Initializes the matrix dimensions and populates sparse_data_ using the update() method.

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

// 🔥 CORE METHODS
template<typename T, StorageOrder Order>
bool Matrix<T, Order>::update(const size_t i, const size_t j, const T& value) {
// Updates the value at position (i, j) in the uncompressed (sparse_data_) format.
// Inserts or updates the value if it's non-zero; removes the entry if the value is zero.
 
    std::array<size_t, 2> key = {i, j};
    if (value != T(0)) {
        sparse_data_[key] = value;
    } else {
        sparse_data_.erase(key);
    }
    return true;
}

template<typename T, StorageOrder Order>
void Matrix<T, Order>::compress() {
// Converts the matrix from uncompressed (Coo-MAP) format to compressed format (CSR for RowMajor, CSC for ColMajor).
// It builds compressed_data_ from sparse_data_ and then clears the uncompressed storage to save memory.

    // Determine the conversion type
    constexpr bool isRowMajor = (Order == StorageOrder::RowMajor);
    size_t outer_size = isRowMajor ? rows_ : cols_;

    // Clears the values and preparation
    compressed_data_.values.clear();
    compressed_data_.inner_index.clear();
    compressed_data_.outer_ptr.assign(outer_size + 1, 0);  // initialise outer_ptr with zeroes

    // 1. Counts the values in each row/column
    for (const auto& [key, val] : sparse_data_) {
        size_t outer = isRowMajor ? key[0] : key[1];  // if CSR uses the row as outer if CSC uses the column  
        compressed_data_.outer_ptr[outer + 1]++;   // to know how many elements are in each column/row
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
    std::vector<size_t> temp_offset = compressed_data_.outer_ptr;

    // 4. Giving values to the vectors
    for (const auto& [key, val] : sparse_data_) {
        size_t outer = isRowMajor ? key[0] : key[1];
        size_t inner = isRowMajor ? key[1] : key[0];
        size_t idx = temp_offset[outer]++;

        compressed_data_.values[idx] = val;
        compressed_data_.inner_index[idx] = inner;
    }

    sparse_data_.clear();

}

template<typename T, StorageOrder Order>
void Matrix<T, Order>::decompress() {
// Converts the matrix from compressed form (CSR/CSC) to uncompressed form (Coo-MAP).
// It reconstructs sparse_data_ using the compressed_data_ arrays and clears the compressed storage afterward.

    sparse_data_.clear();

    constexpr bool isRowMajor = (Order == StorageOrder::RowMajor);
    size_t outer_size = compressed_data_.outer_ptr.size() - 1;

    for (size_t outer = 0; outer < outer_size; ++outer) {
        for (size_t k = compressed_data_.outer_ptr[outer]; k < compressed_data_.outer_ptr[outer + 1]; ++k) {
            size_t inner = compressed_data_.inner_index[k];

            size_t i = isRowMajor ? outer : inner;
            size_t j = isRowMajor ? inner : outer;

            update(i, j, compressed_data_.values[k]);  // utilizzo del metodo update()
        }
    }

    compressed_data_.clear();
}

template<typename T, StorageOrder Order>
template<NormType norm_type>
T Matrix<T, Order>::norm() {
// Computes the matrix norm (One, Infinity, or Frobenius) depending on the template parameter.
// Handles both compressed (CSR/CSC) and uncompressed (COOmap) storage formats, with automatic transposition if needed.
// Returns a scalar value representing the computed norm.
    
    T norm = T(0);
    if (is_compressed()){
        // Compressed case - CSR/CSC
        
        bool was_transposed = false;
        if constexpr (norm_type == NormType::One) {
            if constexpr (Order == StorageOrder::RowMajor){
                transpose();
                was_transposed=true;
            }
            for (size_t j = 0; j < cols_; ++j) {
                T col_sum = T(0);
                for (size_t idx = compressed_data_.outer_ptr[j]; idx < compressed_data_.outer_ptr[j+1]; ++idx) {
                    col_sum += std::abs(compressed_data_.values[idx]);
                }
                if (std::abs(col_sum) > std::abs(norm)) {norm = col_sum;}
            }
            if(was_transposed){transpose();}
            return norm;
            
        } else if constexpr (norm_type == NormType::Infinity) {
            if constexpr (Order == StorageOrder::ColumnMajor){
                transpose();
                was_transposed=true;
            }
            for (size_t j = 0; j < cols_; ++j) {
                T row_sum = T(0);
                for (size_t idx = compressed_data_.outer_ptr[j]; idx < compressed_data_.outer_ptr[j+1]; ++idx) {
                    row_sum += std::abs(compressed_data_.values[idx]);
                }
                if (std::abs(row_sum) > std::abs(norm)) {norm = row_sum;}
            }
            if(was_transposed){transpose();}
            return norm;
        } else if constexpr (norm_type == NormType::Frobenius) {
            if constexpr (Order == StorageOrder::ColumnMajor){
                transpose();
                was_transposed=true;
            }
            for (size_t j = 0; j < cols_; ++j) {
                T row_sum = T(0);
                for (size_t idx = compressed_data_.outer_ptr[j]; idx < compressed_data_.outer_ptr[j+1]; ++idx) {
                    row_sum += std::pow(std::abs(compressed_data_.values[idx]),T(2));
                }
                norm += row_sum;
            }
            if(was_transposed){transpose();}
            return std::sqrt(norm);
        }
    }
    else {
        // TO DO
        // Uncompressed case - COOmap
        if constexpr (norm_type == NormType::One) {
            std::vector<T> col_sums(cols_, T(0)); // empty vector of dimension cols_
            for(const auto& [coord, value] : sparse_data_) {
                col_sums[coord[1]] += std::abs(value);    
            }
            for (const auto& sum : col_sums) {
                if (std::abs(sum) > std::abs(norm)) { norm = sum; }
                // needed to support also std::complex...
            }
            return norm;
        } else if constexpr (norm_type == NormType::Infinity) {
            std::vector<T> row_sums(rows_, T(0)); // empty vector of dimension rows_
            for(const auto& [coord, value] : sparse_data_) {
                row_sums[coord[0]] += std::abs(value);    
            }
            for (const auto& sum : row_sums) {
                if (std::abs(sum) > std::abs(norm)) { norm = sum; }
                // needed to support also std::complex...
            }
            return norm;
        } else if constexpr (norm_type == NormType::Frobenius) {
            for(const auto& [coord, value] : sparse_data_){
                norm += std::pow(std::abs(value), T(2));
            }
            return std::sqrt(norm);
        }
    }
    
}

template<typename T, StorageOrder Order>
void Matrix<T, Order>::transpose() {
// Transposes the matrix in-place by swapping rows and columns.
// Updates the sparse data (non-zero entries) by flipping their indices.
// Recompresses the matrix if it was previously compressed before transposing.

    // If compressed, decompress first
    bool was_compressed = false;
    if (is_compressed()) {
        decompress();
        was_compressed = true;
    }

    // New sparse_data where we will store transposed entries
    std::map<std::array<size_t, 2>, T> new_sparse_data;

    for (const auto& [key, value] : sparse_data_) {
        std::array<size_t, 2> transposed_key = {key[1], key[0]}; // flip (i,j) -> (j,i)
        new_sparse_data[transposed_key] = value;
    }

    // Swap rows and columns
    std::swap(rows_, cols_);

    // Replace old sparse data with the transposed one
    sparse_data_ = std::move(new_sparse_data);

    // If it was compressed in the beginning, recompress it
    if(was_compressed){compress();}
}

// Product by Vector
template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::compressed_product_by_vector_parallel(const std::vector<T>& v) const {
// Multiplies a compressed matrix by a vector v using parallelization for faster computation.
// Uses OpenMP to parallelize the dot product calculation for each row of the matrix.
// Inputs: v - a vector of type T, Outputs: a vector of type T with the result of the multiplication.

    std::vector<T> output(rows_, T(0));

    if constexpr (Order == StorageOrder::RowMajor) {
        // RowMajor (CSR): traverse row by row (parallelized)
        #pragma omp parallel for
        for (size_t i = 0; i < rows_; ++i) {
            T sum = 0;
            for (size_t k = compressed_data_.outer_ptr[i]; k < compressed_data_.outer_ptr[i + 1]; ++k) {
                sum += compressed_data_.values[k] * v[compressed_data_.inner_index[k]];
            }
            output[i] = sum;
        }
    } else {
        // ColumnMajor (CSC): traverse column by column
        #pragma omp parallel for
        for (size_t j = 0; j < cols_; ++j) {
            for (size_t k = compressed_data_.outer_ptr[j]; k < compressed_data_.outer_ptr[j + 1]; ++k) {
                size_t i = compressed_data_.inner_index[k];
                #pragma omp atomic
                output[i] += compressed_data_.values[k] * v[j];
            }
        }
    }

    return output;
}

// Compressed product by vector (no parallel version)
template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::compressed_product_by_vector(const std::vector<T>& v) const {
    std::vector<T> output(rows_, T(0));

    if constexpr (Order == StorageOrder::RowMajor) {
        // RowMajor (CSR): traverse row by row
        for (size_t i = 0; i < rows_; ++i) {
            T sum = 0;
            for (size_t k = compressed_data_.outer_ptr[i]; k < compressed_data_.outer_ptr[i + 1]; ++k) {
                sum += compressed_data_.values[k] * v[compressed_data_.inner_index[k]];
            }
            output[i] = sum;
        }
    } else {
        // ColumnMajor (CSC): traverse column by column
        for (size_t j = 0; j < cols_; ++j) {
            for (size_t k = compressed_data_.outer_ptr[j]; k < compressed_data_.outer_ptr[j + 1]; ++k) {
                size_t i = compressed_data_.inner_index[k];
                output[i] += compressed_data_.values[k] * v[j];
            }
        }
    }

    return output;
}



template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::product_by_vector(const std::vector<T>& v) const {
// Multiplies the matrix by a vector v. If the matrix is compressed, it uses either parallel or regular multiplication based on the number of rows. 
// For uncompressed matrices in COO format, it performs the multiplication by iterating over sparse data.
// Inputs: v - a vector of type T, Outputs: a vector of type T with the result of the multiplication.

    if (is_compressed()) {
        if constexpr (Order==StorageOrder::RowMajor){
            if (rows_>=params::NROWS_PARALLELIZATON_LIMIT){
            return compressed_product_by_vector_parallel(v);
            }
            else{
                return compressed_product_by_vector(v);
            }
        }
        else{
            if (rows_>=params::NCOLS_PARALLELIZATON_LIMIT){
            return compressed_product_by_vector_parallel(v);
            }
            else{
                return compressed_product_by_vector(v);
            }
        }
        
    }
    else {
        // Create the output vector 
        std::vector<T> output(rows_, 0);

        // Uncompressed multiplication (COO)
        for (const auto& [key, val] : sparse_data_) {
            size_t i = key[0];
            size_t j = key[1];
            output[i] += val * v[j];
        }
        return output;
    }

}

template<typename T, StorageOrder Order> // This function applies to any Matrix type with any storage order
std::vector<T> Matrix<T, Order>::operator*(const Matrix<T, Order>& rhs) const {// The result is a plain std::vector<T>, representing the product result
// Overloads the * operator to perform matrix-vector multiplication when rhs is a column vector matrix.
// Assumes rhs is effectively a column vector; converts it to std::vector<T> and uses product_by_vector for the computation.
// Ensures rhs is in uncompressed form before extracting values.

    //Check dimension compatibility
    if (cols_ != rhs.rows_) { 
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    //Convert rhs into a std::vector<T>
    std::vector<T> vec(rhs.rows_, 0);

   // Make sure rhs is decompressed
    if (rhs.is_compressed()) {
        Matrix<T, Order> rhs_copy = rhs;// We cannot modify rhs (it's a const reference), so we create a copy
        rhs_copy.decompress();
        for (const auto& [key, val] : rhs_copy.sparse_data_) {
            vec[key[0]] = val; // key[0] represents the row index (since rhs is a column vector)
        }
    } else {
        for (const auto& [key, val] : rhs.sparse_data_) {
            vec[key[0]] = val; // If already uncompressed, directly extract values from sparse_data_
        }
    }
    // Use existing method to perform matrix-vector multiplication
    return this->product_by_vector(vec);
}

// MATRIX MARKET PARSER + LOADER METHODS
template<typename T, StorageOrder Order>
bool Matrix<T, Order>::mm_stringstream_to_sparsedata_loader(const std::istringstream& iss_original){
// Parses a Matrix Market file's content (from a stringstream) into sparse matrix data format.
// Reads the header to get matrix dimensions, then processes triplets (row, column, value) to populate the matrix.
// Inputs: iss_original - an input stringstream containing the file content, Outputs: true if parsing is successful, false otherwise.

    std::istringstream iss(iss_original.str()); // fai una copia per poter usare getline e >> separatamente
    std::string line;
    int max_row_idx = 0;
    int max_col_idx = 0;

    // Skip header and comments
    while (std::getline(iss, line)) {
        if (line.empty() || line[0] == '%') continue;
        else break; // first non-comment line
    }

    // parse dimensions
    std::istringstream header_line(line);
    size_t rows, cols, entries;
    if (!(header_line >> rows >> cols >> entries)) {
        std::cerr << "Error parsing header line: " << line << std::endl;
        return false;
    }

    // parse triplets
    int row, col;
    double value;
    while (iss >> row >> col >> value) {
        if (row > max_row_idx) max_row_idx = row;
        if (col > max_col_idx) max_col_idx = col;
        update(row - 1, col - 1, value); // Matrix Market is 1-based
    }

    rows_ = static_cast<size_t>(max_row_idx);
    cols_ = static_cast<size_t>(max_col_idx);

    return true;
}

template<typename T, StorageOrder Order>
std::string Matrix<T, Order>::mm_extract_gz(const std::string& filename){
// Extracts and reads the contents of a compressed (.gz) Matrix Market file.
// Uses zlib's gz functions to read the file in chunks and accumulate its contents into a string.
// Inputs: filename - the file path, Outputs: the content of the file as a string.

    gzFile file = gzopen(filename.c_str(), "rb");
    if (!file) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return "";
    }
    

    
    char buffer[params::BUFFER_SIZE];

    std::string file_content;

    int bytes_read;
    while ((bytes_read = gzread(file, buffer, params::BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate buffer
        file_content += buffer;    // Accumulate contents
    }

    gzclose(file);
    return file_content;
}

template<typename T, StorageOrder Order>
bool Matrix<T, Order>::mm_load_mtx(const std::string& filename){
// Loads a Matrix Market (.mtx or .mtx.gz) file and parses its contents into sparse data format.
// Supports both compressed (.mtx.gz) and uncompressed (.mtx) Matrix Market files.
// Inputs: filename - the file path, Outputs: true if loading is successful, false otherwise.

    
    if (filename.ends_with(".mtx.gz")) {
        sparse_data_.clear(); // clear sparse_data_ values
        compressed_data_.clear(); // clear compressed data values

        auto file_content = mm_extract_gz(filename);
        std::istringstream iss(file_content);
        return mm_stringstream_to_sparsedata_loader(iss);
    } 
    else if (filename.ends_with(".mtx")) {
        sparse_data_.clear(); // clear sparse_data_ values
        compressed_data_.clear(); // clear compressed data values

        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            std::cerr << "Error: could not open file " << filename << std::endl;
            return false;
        }
        
        // Leggi il contenuto del file direttamente in un istringstream
        std::istringstream iss;
        iss.str(std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()));

        return mm_stringstream_to_sparsedata_loader(iss);
    }
    else{
        std::cout << "Not a Matrix Market file..." << std::endl;
        return false;
    }
    
}
    

template<typename T, StorageOrder Order>
void Matrix<T, Order>::resize(size_t new_rows, size_t new_cols) {
    bool was_compressed = false;
    if (is_compressed()){
        decompress();
        was_compressed = true;
    }
    rows_ = new_rows;
    cols_ = new_cols;

    // Remove entries that are now out of bounds
    for (auto it = sparse_data_.begin(); it != sparse_data_.end(); ) {
        auto [row, col] = it->first;
        if (row >= new_rows || col >= new_cols) {
            it = sparse_data_.erase(it);  // erase returns the next valid iterator
        } else {
            ++it;
        }
    }

    if (was_compressed){
        compress();
    }
}

template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::diagonal_view() const {
// Returns a vector containing the diagonal elements of the matrix.
// If an element on the diagonal is not stored explicitly (i.e., zero in sparse form), we assume it is 0.

    std::vector<T> diag(std::min(rows_, cols_), T(0));

    if (!is_compressed()) {
        // Uncompressed (COO map) case
        for (size_t i = 0; i < diag.size(); ++i) {
            std::array<size_t, 2> key = {i, i};
            auto it = sparse_data_.find(key);
            if (it != sparse_data_.end()) {
                diag[i] = it->second;
            }
        }
    } else {
        // Compressed (CSR/CSC) case
        constexpr bool isRowMajor = (Order == StorageOrder::RowMajor);

        if (isRowMajor) {
            for (size_t i = 0; i < diag.size(); ++i) {
                for (size_t k = compressed_data_.outer_ptr[i]; k < compressed_data_.outer_ptr[i + 1]; ++k) {
                    if (compressed_data_.inner_index[k] == i) {
                        diag[i] = compressed_data_.values[k];
                        break;
                    }
                }
            }
        } else {
            for (size_t j = 0; j < diag.size(); ++j) {
                for (size_t k = compressed_data_.outer_ptr[j]; k < compressed_data_.outer_ptr[j + 1]; ++k) {
                    if (compressed_data_.inner_index[k] == j) {
                        diag[j] = compressed_data_.values[k];
                        break;
                    }
                }
            }
        }
    }

    return diag;
}



// ℹ️ INFO & PRINTING METHODS
template<typename T, StorageOrder Order>
void Matrix<T, Order>::print(int width) const {
    // Prints the matrix in a tabular, human-readable form.
    // If the matrix is uncompressed, it prints from sparse_data_.
    // If compressed, it reconstructs the row-wise representation using compressed_data_.
    // Parameters:
    //   width (optional): number of characters per column (default = 6, see Matrix.hpp).

    if (!is_compressed()) {
        // Uncompressed mode
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                std::array<size_t, 2> key = {i, j};
                if (sparse_data_.count(key)) {
                    std::cout << std::setw(width) << sparse_data_.at(key);
                } else {
                    std::cout << std::setw(width) << T(0);
                }
            }
            std::cout << '\n';
        }
    } else {
        // Compressed mode
        for (size_t i = 0; i < compressed_data_.outer_ptr.size() - 1; ++i) {
            auto row_start = compressed_data_.outer_ptr[i];
            auto row_end = compressed_data_.outer_ptr[i + 1];

            std::unordered_map<size_t, T> row_values;
            for (size_t idx = row_start; idx < row_end; ++idx) {
                row_values[compressed_data_.inner_index[idx]] = compressed_data_.values[idx];
            }

            for (size_t j = 0; j < cols_; ++j) {
                if (row_values.count(j)) {
                    std::cout << std::setw(width) << row_values[j];
                } else {
                    std::cout << std::setw(width) << T(0);
                }
            }
            std::cout << '\n';
        }
    }
}

template<typename T, StorageOrder Order>
void Matrix<T, Order>::printStorage() const {
// Prints the storage format of the matrix (compressed or uncompressed).
// Displays the compressed sparse representation (CSR/CSC) or the uncompressed COO format, showing values, indices, and pointers.
// Outputs the matrix storage details to the console.

    if(is_compressed()){
        // Print CSR/CSC vectors
        std::cout << std::string(50, '-') << "\n";
        std::cout << "         Compressed Sparse Representation \n";
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
    else{
        // Print COOmap Representation
        std::cout << std::string(50, '-') << "\n";
        std::cout << "    Uncompressed Sparse Representation (Coo-MAP)\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (const auto& [key, val] : sparse_data_) {
            std::cout << "Key: (" << key[0] << ", " << key[1]
            << ") -> Value: " << val << std::endl;
        }

        std::cout << "\n";
        std::cout << std::string(50, '-') << "\n";
    }
}

template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::weight() const{
// Calculates the memory usage (weight) of the matrix based on its storage format.
// For compressed matrices, it sums the sizes of the values, indices, and pointers; for uncompressed, it estimates based on the sparse data structure.
// Outputs: the memory size in bytes.

    if (is_compressed()){
        return compressed_data_.values.size() * sizeof(T)+ compressed_data_.inner_index.size() * sizeof(size_t)+ compressed_data_.outer_ptr.size() * sizeof(size_t);

    }
    else{
        size_t size_per_element = sizeof(std::array<size_t, 2>) + sizeof(T) + 3 * sizeof(void*) + sizeof(bool);
        return sizeof(sparse_data_) + sparse_data_.size() * size_per_element;
    }
}

template<typename T, StorageOrder Order>
bool Matrix<T, Order>::is_compressed() const{
// Checks whether the matrix is in compressed form (CSR/CSC).
// Returns true if all three components of compressed_data_ are non-empty.

   return compressed_data_.values.size() != 0 && compressed_data_.inner_index.size() != 0 && compressed_data_.outer_ptr.size() != 0;
}

template<typename T, StorageOrder Order>
void Matrix<T, Order>::info() const {
    std::cout << std::string(50, '*') << std::endl;
    std::cout << "*           Matrix Information Summary           *" << std::endl;
    std::cout << std::string(50, '*') << std::endl;
    std::cout << std::left;
    std::cout << std::setw(30) << "  Size:" << rows_ << " x " << cols_ << std::endl;
    std::cout << std::setw(30) << "  Storage Order:" << storageOrderToString(Order) << std::endl;
    std::cout << std::setw(30) << "  Element Type:" << utils::demangle(typeid(T).name()) << std::endl;
    std::cout << std::setw(30) << "  Compression status:" << (is_compressed() ? "Compressed" : "Uncompressed") << std::endl;
    std::cout << std::setw(30) << "  Memory usage (bytes):" << weight() << std::endl;
    std::cout << std::string(50, '*') << std::endl;
}


// ✝️ GRAVEYARD : DEPRECATED FUNCTIONS
// ashes have been scattered, nothing to see here


} // namespace algebra