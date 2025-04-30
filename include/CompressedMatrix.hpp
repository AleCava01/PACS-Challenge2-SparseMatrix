#ifndef COMPRESSEDMATRIX_HPP
#define COMPRESSEDMATRIX_HPP

namespace algebra{

/**
 * @brief Structure for storing a sparse matrix in compressed format (CSR or CSC).
 * 
 * @tparam T Type of the matrix elements.
 * 
 * This structure holds the data needed for a compressed sparse row (CSR) or 
 * compressed sparse column (CSC) representation: the nonzero values, the 
 * inner indices (column indices in CSR or row indices in CSC), and the 
 * outer pointers (row pointers in CSR or column pointers in CSC).
 */
template<typename T>
struct CompressedMatrix {
    /**
     * @brief Vector containing the nonzero values of the matrix.
     */
    std::vector<T> values;        

    /**
     * @brief Vector containing the column indices (in CSR) or row indices (in CSC) for each nonzero value.
     */
    std::vector<size_t> inner_index;

    /**
     * @brief Vector containing the starting positions of each row (in CSR) or column (in CSC) in the values array.
     */
    std::vector<size_t> outer_ptr;

    /**
     * @brief Clears all vectors and deallocates their memory.
     */
    void clear() { 
        std::vector<T>().swap(values);
        std::vector<size_t>().swap(inner_index);
        std::vector<size_t>().swap(outer_ptr);
    }

    /**
     * @brief Retrieves the value at the specified position (i, j).
     * 
     * @param i Row index.
     * @param j Column index.
     * @param order Storage order (RowMajor or ColumnMajor).
     * @return T Value at position (i, j), or zero if the element is not stored (i.e., it is implicitly zero).
     */
    T get(size_t i, size_t j, StorageOrder order) const {
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
