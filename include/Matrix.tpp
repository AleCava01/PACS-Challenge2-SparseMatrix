#include "Matrix.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <vector>
#include <omp.h>

namespace algebra{

// METHODS IMPLEMENTATION OF MATRIX CLASS

// Matrix constructor (empty matrix initialization)
template<typename T, StorageOrder Order>
Matrix<T, Order>::Matrix(size_t rows, size_t cols){
    rows_ = rows;
    cols_ = cols;
}

// Matrix constructor (matrix initialization)
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

// Matrix print function
template<typename T, StorageOrder Order>
void Matrix<T, Order>::print() const {
    if (!is_compressed()){ 
        // Print uncompressed matrix (sparse_data_)
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                std::array<size_t, 2> key = {i, j};
                if (sparse_data_.count(key)) {
                    std::cout << sparse_data_.at(key) << " ";
                } else {
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    }
    else{
        // Print compressed matrix (compressed_data_)
        auto k = 0;
        for(size_t i=0; i<compressed_data_.outer_ptr.size()-1; ++i){
            if (compressed_data_.outer_ptr[i]!=compressed_data_.outer_ptr[i+1])
            {
                auto num_values = compressed_data_.outer_ptr[i+1]-compressed_data_.outer_ptr[i];
                for(size_t j=0; j<cols_; ++j){
                    if(compressed_data_.inner_index[k]==j && num_values>0){
                        std::cout<<compressed_data_.values[k]<< " ";
                        k++;
                        num_values--;
                    }
                    else{
                        std::cout<<"0 ";
                    }
                }
                std::cout<<std::endl;
            }
            else{
                for(size_t j=0; j<cols_;++j){std::cout<<"0 ";}
                std::cout<<std::endl;
            }     
        }
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
    std::cout << std::setw(30) << "  Compression status:" << (is_compressed() ? "Compressed" : "Uncompressed") << std::endl;
    std::cout << std::setw(30) << "  Memory usage (bytes):" << weight() << std::endl;
    std::cout << std::string(50, '*') << std::endl;
}

// update(i,j,value) for sparse matrix if the matrix is dynamically stored as COOmap
template<typename T, StorageOrder Order>
bool Matrix<T, Order>::update(const size_t i, const size_t j, const T& value) {
    std::array<size_t, 2> key = {i, j};
    if (value != T(0)) {
        sparse_data_[key] = value;
    } else {
        sparse_data_.erase(key);
    }
    return true;
}

// Compression
// matrix compression (from COOmap to CSR/CSC)
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

// Decompression
// matrix decompression (from CSR/CSC to COOmap)
template<typename T, StorageOrder Order>
void Matrix<T, Order>::decompress() {

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

// checks if the Storage is with the compressed or uncompressed method
// returns compression status
template<typename T, StorageOrder Order>
bool Matrix<T, Order>::is_compressed() const{
   return compressed_data_.values.size() != 0 && compressed_data_.inner_index.size() != 0 && compressed_data_.outer_ptr.size() != 0;
}

// Multiplication by vector
template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::product_by_vector(const std::vector<T>& v) const {

    // Create the output vector 
    std::vector<T> output(rows_, 0);

    if(is_compressed()){
        // Compressed matrix * vector multiplication

        std::vector<size_t> row_offset(rows_ + 1, 0); // stores the starting indexes to iterate over compressed_data_.values
        // the row_offset vector will look like:
        // [0, number_of_elements_in_the_first_row, number_of_elements_in_the_first_2_rows, ...]
        for (size_t i = 0; i < rows_; ++i) {
            row_offset[i+1] = row_offset[i] + compressed_data_.outer_ptr[i+1] - compressed_data_.outer_ptr[i];
        }
        // enabling parallel computing
        #pragma omp parallel for
        // Iterate over matrix rows
        for (size_t i = 0; i < rows_ ; i++){
            
            // Extract matrix row vector
            std::vector<T> matrix_row = extract_row(i,row_offset[i]);

            // Compute the dot product between row and the input vector
            T result = matrix_row * v; // * operator has been overloaded in Utils.hpp

            // update the output vector
            output[i]=result;
        }
    }
    else{
        //std::cout << "[DEBUG] Using uncompressed multiplication..." << std::endl;
        for(const auto& [key, val] : sparse_data_){
            size_t i = key[0];
            size_t j = key[1];
            output[i] += val * v[j]; // * operator has been overloaded in Utils.hpp
        }

    }

    
    return output;
}

// operator* for Matrix * (Matrix with one column)
template<typename T, StorageOrder Order> // This function applies to any Matrix type with any storage order
std::vector<T> Matrix<T, Order>::operator*(const Matrix<T, Order>& rhs) const {// The result is a plain std::vector<T>, representing the product result

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




// Extract the specified row when the matrix is stored as CSR/CSC matrix.
template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::extract_row(size_t index, size_t k) const{
    std::vector<T> row(cols_, 0);

    if (compressed_data_.outer_ptr[index]!=compressed_data_.outer_ptr[index+1])
    {
        auto num_values = compressed_data_.outer_ptr[index+1]-compressed_data_.outer_ptr[index];
        for(size_t j=0; j<cols_; ++j){
            if(compressed_data_.inner_index[k]==j && num_values>0){
                row[j] = compressed_data_.values[k];
                k++;
                num_values--;
            }
        }
    }
    return row;
}

// Print Storage
// prints CSR/CSC vectors or COOmap mapping, depending on compression status
template<typename T, StorageOrder Order>
void Matrix<T, Order>::printStorage() const {
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

// Returns the matrix space usage, in bytes
template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::weight() const{
    if (is_compressed()){
        return (compressed_data_.values.size()+compressed_data_.inner_index.size()+compressed_data_.outer_ptr.size()) * sizeof(T);
    }
    else{
        size_t size_per_element = sizeof(std::array<size_t, 2>) + sizeof(T) + 3 * sizeof(void*) + sizeof(bool);
        return sizeof(sparse_data_) + sparse_data_.size() * size_per_element;
    }
}

} // namespace algebra