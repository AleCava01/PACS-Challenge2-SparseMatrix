#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

/**
 * @file Parameters.hpp
 * @brief Defines global constants used throughout the program.
 * 
 * This header provides constants that control parallelization thresholds 
 * and buffer sizes for file operations.
 */

namespace params {

/**
 * @brief Row threshold for enabling parallelization.
 * 
 * When the number of rows exceeds this limit, parallel processing is applied.
 */
const int NROWS_PARALLELIZATON_LIMIT = 1000;

/**
 * @brief Column threshold for enabling parallelization.
 * 
 * When the number of columns exceeds this limit, parallel processing is applied.
 */
const int NCOLS_PARALLELIZATON_LIMIT = 1000;

/**
 * @brief Size of the buffer used in file operations.
 * 
 * Specifies the buffer size (in bytes) used for operations such as reading from files,
 * including gzip file decompression.
 */
constexpr int BUFFER_SIZE = 8192;

} // namespace params

#endif // PARAMETERS_HPP
