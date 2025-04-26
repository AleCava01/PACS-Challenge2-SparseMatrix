#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

namespace algebra{

// To store the 3 vectors for the CSR/CSC storage 
template<typename T>
struct CompressedMatrix {
    std::vector<T> values;        // Contains non zero values
    std::vector<size_t> inner_index; // Indexes of rows/columns
    std::vector<size_t> outer_ptr;   // Pointers to the starting row/column
    
    void clear() { 
        // clear vectors deallocating memory
        std::vector<T>().swap(values);
        std::vector<size_t>().swap(inner_index);
        std::vector<size_t>().swap(outer_ptr);
    }
    T get(size_t i, size_t j, StorageOrder order) const {
        // returns the element at position i,j depending on the StorageOrder
        if (order == StorageOrder::RowMajor) {
            size_t row_start = outer_ptr[i];
            size_t row_end = outer_ptr[i+1];
            for (size_t idx = row_start; idx < row_end; ++idx) {
                if (inner_index[idx] == j) {
                    return values[idx];
                }
            }
            return T(0); // If not found, it's zero
        } else { // Column Major
            size_t col_start = outer_ptr[j];
            size_t col_end = outer_ptr[j+1];
            for (size_t idx = col_start; idx < col_end; ++idx) {
                if (inner_index[idx] == i) {
                    return values[idx];
                }
            }
            return T(0); // If not found, it's zero
        }
    }
};

} // namespace algebra

#endif // COMPRESSEDMATRIX_HPP