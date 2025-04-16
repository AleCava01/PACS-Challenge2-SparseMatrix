#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

namespace algebra{

// To store the 3 vectors for the CSR/CSC storage 
template<typename T>
struct CompressedMatrix {
    std::vector<T> values;        // Contains non zero values
    std::vector<size_t> inner_index; // Indexes of rows/columns
    std::vector<size_t> outer_ptr;   // Pointers to the starting row/column
    
    void clear() { // clear vectors deallocating memory
        std::vector<T>().swap(values);
        std::vector<size_t>().swap(inner_index);
        std::vector<size_t>().swap(outer_ptr);
    }
};

} // namespace algebra

#endif // COMPRESSEDMATRIX_HPP