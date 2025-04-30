#ifndef TESTS_HPP
#define TESTS_HPP

#include <fstream>
#include <iostream>
#include <chrono>
#include <random> 
#include <tuple>
#include <cxxabi.h>
#include <numeric>

#include "Matrix.hpp"
#include "Utils.hpp"

using namespace utils;

namespace tests{
    // Multiplication testing utilities
    /**
     * @brief Measures the execution time of matrix-vector multiplication.
     * 
     * This function performs matrix-vector multiplication using the provided matrix 
     * and vector, and returns both the result and the duration of the operation in microseconds.
     * 
     * @tparam T The data type of the matrix and vector elements (e.g., int, double).
     * @tparam Order The storage order of the matrix (either RowMajor or ColumnMajor).
     * 
     * @param mat The matrix used in the multiplication.
     * @param v The vector to multiply with the matrix.
     * 
     * @return A tuple containing:
     * - The resulting vector of the multiplication.
     * - The duration of the multiplication in microseconds.
     */
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication(const Matrix<T, Order>& mat, const std::vector<T>& v);
    /**
     * @brief Measures the average execution time of matrix-vector multiplication over multiple repetitions.
     * 
     * This function performs matrix-vector multiplication multiple times, calculates the 
     * average execution time, and returns both the last result and the average duration.
     * 
     * @tparam T The data type of the matrix and vector elements (e.g., int, double).
     * @tparam Order The storage order of the matrix (either RowMajor or ColumnMajor).
     * 
     * @param mat The matrix used in the multiplication.
     * @param v The vector to multiply with the matrix.
     * @param repetitions The number of repetitions to perform.
     * 
     * @return A tuple containing:
     * - The last result from the matrix-vector multiplication.
     * - The average duration of the multiplication across all repetitions in microseconds.
     */
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication_mean(const Matrix<T, Order>& mat, const std::vector<T>& v, int repetitions = 10);
    /**
     * @brief Generates a fixed test matrix for validation or testing purposes.
     * 
     * This function returns a 4x4 fixed test matrix used for unit tests or other specific
     * use cases where a known matrix is required. The matrix is predefined with integer elements.
     * 
     * @tparam T The data type of the matrix elements (e.g., int, double).
     * 
     * @return A 2D vector representing the fixed test matrix.
     */
    template<typename T>
    std::vector<std::vector<T>> get_fixed_test_matrix();

    // Speed tests
    /**
     * @brief Compares the performance of compressed vs. uncompressed matrix-vector multiplication.
     * 
     * Measures and prints the time (in milliseconds) taken for multiplying a sparse matrix 
     * (both compressed and uncompressed) by a vector across various matrix sizes.
     * 
     * Matrix sizes tested: {100, 200, ..., 1000}.
     * 
     * @note Results are printed to the console.
     */
    void multiplication_compressed_vs_uncompressed_speedtest();
    /**
     * @brief Benchmarks compressed matrix-vector multiplication with and without parallelization.
     * 
     * Measures and prints the execution time (in milliseconds) for matrix-vector multiplication 
     * using compressed storage, comparing single-threaded vs. parallelized computation.
     * 
     * @param num_iterations Number of times the benchmark should be repeated for each matrix size.
     * 
     * @details
     * Matrix sizes tested: {100, 200, ..., 2000}.
     * 
     * Results are printed to the console and saved to "output/performance_results.csv".
     */
    void multiplication_compressed_parallel_vs_unparallel_speedtest(size_t num_iterations = 5);
    /**
     * @brief Full benchmark comparing multiple matrix storage and computation strategies.
     * 
     * Measures and prints the time (in milliseconds) taken to perform matrix-vector multiplication 
     * across:
     *  - Compressed RowMajor storage
     *  - Compressed ColumnMajor storage
     *  - Uncompressed RowMajor storage
     *  - Uncompressed ColumnMajor storage
     * 
     * @param num_iterations Number of iterations to run for each matrix size.
     * 
     * @details
     * Matrix sizes tested: {100, 200, ..., 2000}.
     * 
     * Results are printed to the console in a formatted table.
     */
    void multiplication_all_speedtest(size_t num_iterations=1);
    /**
     * @brief Benchmarking the performance of matrix-vector multiplication for Matrix Market files.
     * 
     * Loads a sparse matrix from a Matrix Market file, performs matrix-vector multiplication
     * using both compressed and uncompressed storage formats, and compares the execution times.
     * 
     * @param filename The path to the Matrix Market (.mtx) file containing the sparse matrix.
     * 
     * @details
     * The matrix is first loaded from the file and a random vector of appropriate size is generated.
     * Then, the multiplication is performed both with the compressed matrix and the uncompressed matrix.
     * The time taken for each multiplication is measured and printed to the console.
     * 
     * The results include:
     * - Time taken for compressed matrix-vector multiplication
     * - Time taken for uncompressed matrix-vector multiplication
     * 
     * Results are printed in milliseconds with a precision of 3 decimal places.
     */
    void matrix_market_speedtest(const std::string& filename);

    // Simple Matrix * Vector tests
    /**
     * @brief Tests matrix-vector multiplication on both compressed and uncompressed sparse matrices.
     * 
     * This function initializes a sparse matrix and a vector, then performs matrix-vector multiplication 
     * for both compressed and uncompressed versions of the matrix. It measures and outputs the execution time
     * for each phase (compressed and uncompressed).
     * 
     * @details 
     * The matrix is created using a random sparse matrix generator, and the vector is generated 
     * with random values. The test is repeated for both the compressed and uncompressed versions of the matrix.
     * 
     * @note The multiplication results and execution times are printed to the console.
     */
    void matrix_vector_multiplication_test_1();
    /**
     * @brief Tests matrix-vector multiplication on a complex-valued sparse matrix and vector.
     * 
     * This function performs matrix-vector multiplication on a sparse matrix containing complex numbers
     * and a complex-valued vector. It compresses the matrix before the multiplication, prints the matrix, 
     * vector, and multiplication results, and measures the execution time.
     * 
     * @details 
     * A random sparse matrix with complex values is created, and a complex vector is generated. The matrix 
     * is then compressed before performing the matrix-vector multiplication. The test also outputs the result
     * and execution time for the multiplication.
     * 
     * @note The multiplication result and execution time are printed to the console.
     */
    void complex_matrix_times_vector_test();

    // Specific methods tests
    /**
     * @brief Tests the computation of various matrix norms on both compressed and uncompressed matrices.
     * 
     * This function calculates and prints the One norm, Infinity norm, and Frobenius norm of a matrix. 
     * It tests both the compressed and uncompressed versions of the matrix.
     * 
     * @details 
     * The matrix is either generated randomly with a given density or initialized with a fixed test matrix 
     * based on the `use_fixed_matrix` parameter. The norms are computed using the Matrix class's templated 
     * `norm` function, and results are printed to the console for both compressed and uncompressed matrices.
     * 
     * @tparam T The type of the matrix elements (e.g., int, double, etc.).
     * @tparam Order The storage order of the matrix (e.g., RowMajor, ColumnMajor).
     * 
     * @param use_fixed_matrix A boolean flag indicating whether to use a fixed test matrix or a random sparse matrix.
     * @param rows The number of rows for a randomly generated matrix.
     * @param cols The number of columns for a randomly generated matrix.
     * @param density The sparsity coefficient for a randomly generated matrix.
     */
    template<typename T, StorageOrder Order = StorageOrder::ColumnMajor>
    void matrix_norm_test(bool use_fixed_matrix = true, size_t rows = 4, size_t cols = 4, double density = 0.8);
    /**
     * @brief Tests the matrix resizing functionality.
     * 
     * This function tests the ability to resize a matrix. The matrix is first initialized with specific values, 
     * resized, and the results are printed before and after the resize operation.
     * 
     * @details 
     * A 3x3 matrix is created with specific values. After updating some entries, the matrix is resized to 4x5,
     * and both the original and resized matrices are printed along with their information.
     */
    void matrix_resize_test();
    /**
     * @brief Tests the diagonal view functionality on both compressed and uncompressed matrices.
     * 
     * This function demonstrates extracting the diagonal elements of a matrix. The matrix is first printed, 
     * then its diagonal elements are extracted and printed for both the compressed and uncompressed versions.
     * 
     * @details 
     * A 4x4 matrix is created with specific values, and the diagonal elements are extracted in both compressed 
     * and uncompressed states. The diagonal values are printed for both states to show the difference.
     */
    void diagonal_view_test();

}

#include "Tests.tpp" // Include implementations

#endif // TESTS_HPP