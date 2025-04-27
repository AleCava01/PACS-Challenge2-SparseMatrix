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
#include "Verbose.hpp"

namespace tests{
    void multiplication_compressed_vs_uncompressed_speedtest(){
    // Compares the performance (in milliseconds) of matrix-vector multiplication
    // between compressed and uncompressed sparse matrix representations across various matrix sizes.

        std::cout << "=== Start performance comparison test ===\n\n";

        std::vector<size_t> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

        std::cout << std::left << std::setw(12) << "MatrixSize"
                << std::setw(20) << "Compressed Time (ms)"
                << std::setw(24) << "Uncompressed Time (ms)" << "\n";
        std::cout << std::string(56, '-') << "\n";

        for (size_t size : sizes) {
            Matrix<int, StorageOrder::RowMajor> mat(getRandomSparseMatrix<int>(size, size, 0.95));
            std::vector<int> vec = getRandomVector<int>(size);

            // Compressed
            mat.compress();
            auto start = std::chrono::high_resolution_clock::now();
            auto result1 = mat.product_by_vector(vec);
            auto end = std::chrono::high_resolution_clock::now();
            auto time_compressed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

            // Uncompressed
            mat.decompress();
            start = std::chrono::high_resolution_clock::now();
            auto result2 = mat.product_by_vector(vec);
            end = std::chrono::high_resolution_clock::now();
            auto time_uncompressed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

            // Output
            std::cout << std::left << std::setw(12) << size
                    << std::setw(20) << time_compressed
                    << std::setw(24) << time_uncompressed << "\n";
        }

        std::cout << "\n=== Done ===\n";
    }
    
    void multiplication_compressed_parallel_vs_unparallel_speedtest(size_t num_iterations = 5) {
    // Benchmarks the performance of compressed matrix-vector multiplication
    // with and without parallelization across various matrix sizes and iterations.
    // Results are printed to console and saved to "performance_results.csv".

        std::cout << "=== Start performance comparison test ===\n\n";

        std::vector<size_t> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000, 4000};
        std::ofstream file("output/performance_results.csv");
        file << "Iteration,MatrixSize,Method,TimeMs\n";

        std::cout << std::left << std::setw(12) << "MatrixSize"
                << std::setw(10) << "Iter"
                << std::setw(30) << "Compressed Time (ms)"
                << std::setw(30) << "Compressed Parallel Time (ms)" << "\n";
        std::cout << std::string(90, '-') << "\n";

        for (size_t size : sizes) {
            for (size_t iter = 0; iter < num_iterations; ++iter) {
                Matrix<int, StorageOrder::RowMajor> mat(getRandomSparseMatrix<int>(size, size, 0.95));
                std::vector<int> vec = getRandomVector<int>(size);
                mat.compress();

                // Compressed
                auto start = std::chrono::high_resolution_clock::now();
                auto result1 = mat.compressed_product_by_vector(vec);
                auto end = std::chrono::high_resolution_clock::now();
                double time_compressed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                // Compressed Parallel
                start = std::chrono::high_resolution_clock::now();
                auto result2 = mat.compressed_product_by_vector_parallel(vec);
                end = std::chrono::high_resolution_clock::now();
                double time_parallel = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                // Output to console and file
                std::cout << std::left << std::setw(12) << size
                        << std::setw(10) << iter
                        << std::setw(30) << time_compressed
                        << std::setw(30) << time_parallel << "\n";

                // Write on file
                file << iter << "," << size << ",Unparalleled," << time_compressed << "\n";
                file << iter << "," << size << ",Parallel," << time_parallel << "\n";
            }
        }

        file.close();
        std::cout << "\n=== Done. Results saved to performance_results.csv ===\n";
    }
    
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication(const Matrix<T, Order>& mat, const std::vector<T>& v) {
    // Measures the execution time of the matrix-vector multiplication
    // and returns both the result and the duration (in microseconds).

        auto start = std::chrono::high_resolution_clock::now(); // start time
        std::vector<T> multiplication_result = mat.product_by_vector(v);
        auto end = std::chrono::high_resolution_clock::now(); // end time

        // Duration in microseconds
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return std::make_tuple(multiplication_result, duration);
    }
    
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication_mean(const Matrix<T, Order>& mat, const std::vector<T>& v, int repetitions = 10) {
    // Measures the average execution time of matrix-vector multiplication
    // over a specified number of repetitions and returns the last result and the mean duration.

        long long total_duration = 0;
        std::vector<T> last_result;

        for (int i = 0; i < repetitions; ++i) {
            auto [result, duration] = test_multiplication(mat, v);
            total_duration += duration;
            last_result = std::move(result); // store only the last value
        }

        long long mean_duration = total_duration / repetitions;
        return {last_result, mean_duration};
    }

    template<typename T, StorageOrder Order>
    void test_multiply(const Matrix<T, Order>& mat, const std::vector<T>& v){
        auto multiplication_result = mat.product_by_vector(v);
        verbose::separator(50);
        std::cout << "v: " << std::endl;
        print(v);
        std::cout<<std::endl;
        auto [result, duration] = tests::test_multiplication_mean(mat,v,100);
        verbose::display_mat_times_vector_results(result, duration);
        verbose::separator(50);
    }
    
    void matrix_vector_multiplication_test_1(){
    // This function tests matrix-vector multiplication on both compressed and uncompressed sparse matrices.
    // It initializes a sparse matrix and a vector, performs compression, and runs multiplication tests on both cases.

        // Sparse matrix and vector Initialization
        size_t n_row = 5;
        size_t n_col = 5;
        double sparsity_coeff = 0.9;
        Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix<int>(n_row,n_col,sparsity_coeff));
        std::vector<int> v = getRandomVector<int>(n_row);
        verbose::separator(50);

        // Matrix compression
        mat.compress();
        mat.print();
        mat.info();
        test_multiply(mat, v); // test matrix * vector multiplication / compressed case

        // Matrix decompression
        mat.decompress();
        mat.print();
        mat.info();
        test_multiply(mat, v); // test matrix * vector multiplication / uncompressed case

    }
    
    void matrix_market_speedtest(const std::string& filename) {

        std::cout << "=== Matrix Market Speed Test (" << filename << ") ===\n\n";

        // Load sparse matrix from file
        algebra::Matrix<double, StorageOrder::RowMajor> mat(0, 0);
        if (!mat.mm_load_mtx(filename)) {
            std::cerr << "❌ Failed to load Matrix Market file: " << filename << "\n";
            return;
        }

     
        mat.info();

        // Generate a random vector with length equal to the number of columns
        size_t n = mat.cols();  // This requires the Matrix class to expose a public cols() method.
        std::vector<double> vec(n);
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<> dist(1.0, 9.0);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = dist(gen);
        }

        // Compressed
        mat.compress();
        auto [res_c, time_c] = test_multiplication(mat, vec);

        // Uncompressed
        mat.decompress();
        auto [res_u, time_u] = test_multiplication(mat, vec);

        // Print results
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "\nCompressed Time:   " << (time_c / 1000.0) << " ms\n";
        std::cout << "Uncompressed Time: " << (time_u / 1000.0) << " ms\n";
        std::cout << "=== Done ===\n";
    }

    void complex_matrix_times_vector_test(){
        size_t rows = 4;
        size_t cols = 4;
        auto complex_matrix = getRandomSparseMatrix<std::complex<double>>(rows,cols,0.8);
        auto complex_vector = getRandomVector<std::complex<double>>(rows);
        auto mat = Matrix<std::complex<double>,StorageOrder::RowMajor>(complex_matrix);
        mat.compress();
        mat.print(20);
        test_multiply(mat,complex_vector);
    }
    template<typename T>
    std::vector<std::vector<T>> get_fixed_test_matrix() {
        return {
            {T(1), T(0), T(0), T(0)},
            {T(0), T(0), T(1), T(0)},
            {T(0), T(1), T(0), T(0)},
            {T(0), T(0), T(1), T(3)}
        };
    }
    template<typename T, StorageOrder Order = StorageOrder::ColumnMajor>
    void matrix_norm_test(bool use_fixed_matrix = true, size_t rows = 4, size_t cols = 4, double density = 0.8) {
        Matrix<T, Order> mat(
            use_fixed_matrix 
                ? get_fixed_test_matrix<T>() 
                : getRandomSparseMatrix<T>(rows, cols, density)
        );

        auto compute_and_print_norms = [](Matrix<T, Order>& m, const std::string& label) {
            verbose::separator(50);
            std::cout << label << " matrix:" << std::endl;
            m.print();
            auto one_norm = m.template norm<NormType::One>();
            auto infinity_norm = m.template norm<NormType::Infinity>();
            auto frobenius_norm = m.template norm<NormType::Frobenius>();

            verbose::separator(50);
            std::cout << "One norm:        " << one_norm << std::endl;
            std::cout << "Infinity norm:   " << infinity_norm << std::endl;
            std::cout << "Frobenius norm:  " << frobenius_norm << std::endl;
            verbose::separator(50);
        };

        mat.compress();
        compute_and_print_norms(mat, "Compressed");

        mat.decompress();
        compute_and_print_norms(mat, "Uncompressed");
    }
}



#endif // TESTS_HPP