#include "Matrix.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <vector>

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

// update(i,j,value) for sparse matrix
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

    compressed_data_.inner_index.clear();
    compressed_data_.values.clear();
    compressed_data_.outer_ptr.clear();

}

// checks if the Storage is with the compressed or uncompressed method
template<typename T, StorageOrder Order>
bool Matrix<T, Order>::is_compressed() const{
   return compressed_data_.values.size() != 0 && compressed_data_.inner_index.size() != 0 && compressed_data_.outer_ptr.size() != 0;
}

// Multiplication by vector
template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::product_by_vector(const std::vector<T>& v){

    // Creo il vettore di output 
    std::vector<T> output = {};
    int k = 0;


    // 1. Iterare sulle righe della matirce e estrarre il vettore riga
    // posso usare un metodo tipo quello di print per costruire il vettore 
    for (int i = 0; i < rows_ ; i++){
        
        // Estraggo il vettore riga
        std::vector<T> row_i = extract_row(i, k);

        // faccio il prodotto scalare
        T result = row_i * v;

        output.push_back(result);
    }

    for (int i = 0; i < output.size(); i++){
        std::cout << output[i] << " ";
    }
    std::cout << std::endl << "fine moltiplicazione" << std::endl;
    return output;
}


// funziona 
template<typename T, StorageOrder Order>
std::vector<T> Matrix<T, Order>::extract_row(size_t index, int& k){
    std::vector<T> output = {};

    if (compressed_data_.outer_ptr[index]!=compressed_data_.outer_ptr[index+1])
    {
        auto num_values = compressed_data_.outer_ptr[index+1]-compressed_data_.outer_ptr[index];
        for(size_t j=0; j<cols_; ++j){
            if(compressed_data_.inner_index[k]==j && num_values>0){
                output.push_back(compressed_data_.values[k]);
                k++;
                num_values--;
            }
            else{
                output.push_back(0);
            }
        }
    }
    else{
        for(size_t j=0; j<cols_;++j){
        output.push_back(0);
        }
    }

    for (int i = 0; i < output.size(); i++){
        std::cout << output[i] << " ";
    }
    
    std::cout << std::endl << "fine riga " << std::endl;
    std::cout << std::endl << std::endl;
    return output;
}

// Funzione di supporto che dice in quale riga si trova l'elemento values[k]
template<typename T, StorageOrder Order>
size_t Matrix<T, Order>::find_row_for_index(size_t idx) const {
    if (compressed_data_.outer_ptr.empty() || idx >= compressed_data_.values.size()) {
        throw std::out_of_range("Indice non valido o dati compressi assenti.");
    }

    // CSR: ogni riga ha un intervallo [outer_ptr[i], outer_ptr[i+1])
    for (size_t row = 0; row < rows_; ++row) {
        size_t start = compressed_data_.outer_ptr[row];
        size_t end = compressed_data_.outer_ptr[row + 1];
        if (idx >= start && idx < end) {
            return row;
        }
    }

    throw std::out_of_range("Indice non trovato in nessuna riga.");
}

// Print Compressed Matrix
template<typename T, StorageOrder Order>
void Matrix<T, Order>::compressedInfo() const{

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

// Printing the map
template<typename T, StorageOrder Order>
void Matrix<T, Order>::printSparseData() const {

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

// Returns the matrix weight, in bytes
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