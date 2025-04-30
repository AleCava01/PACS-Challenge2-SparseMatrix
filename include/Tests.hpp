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
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication(const Matrix<T, Order>& mat, const std::vector<T>& v);

    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication_mean(const Matrix<T, Order>& mat, const std::vector<T>& v, int repetitions = 10);
    
    template<typename T>
    std::vector<std::vector<T>> get_fixed_test_matrix();

    // Speed tests
    void multiplication_compressed_vs_uncompressed_speedtest();
    
    void multiplication_compressed_parallel_vs_unparallel_speedtest(size_t num_iterations = 5);
    
    void multiplication_all_speedtest(size_t num_iterations=1);
    
    void matrix_market_speedtest(const std::string& filename);

    // Simple Matrix * Vector tests
    void matrix_vector_multiplication_test_1();

    void complex_matrix_times_vector_test();

    // Specific methods tests
    template<typename T, StorageOrder Order = StorageOrder::ColumnMajor>
    void matrix_norm_test(bool use_fixed_matrix = true, size_t rows = 4, size_t cols = 4, double density = 0.8);

    void matrix_resize_test();

    void diagonal_view_test();


}

#include "Tests.tpp" // Include implementations

#endif // TESTS_HPP