#ifndef STORAGEORDER_HPP
#define STORAGEORDER_HPP

/**
 * @file StorageOrder.hpp
 * @brief Defines storage order types and related utilities for matrices.
 */

namespace algebra {

/**
 * @brief Enumeration for specifying the storage order of a matrix.
 * 
 * - `RowMajor`: Matrix is stored row by row.
 * - `ColumnMajor`: Matrix is stored column by column.
 */
enum class StorageOrder {
    RowMajor,    ///< Row-major storage order.
    ColumnMajor  ///< Column-major storage order.
};

/**
 * @brief Converts a StorageOrder enum value to its corresponding string.
 * 
 * @param order The StorageOrder to convert.
 * @return A C-style string ("RowMajor", "ColumnMajor", or "Unknown" if invalid).
 */
const char* storageOrderToString(StorageOrder order) {
    switch (order) {
        case StorageOrder::RowMajor: return "RowMajor";
        case StorageOrder::ColumnMajor: return "ColumnMajor";
        default: return "Unknown";
    }
}

} // namespace algebra

#endif // STORAGEORDER_HPP
