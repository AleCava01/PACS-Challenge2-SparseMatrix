#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <chrono>
#include <tuple>
#include <random>
#include <complex>
#include <type_traits>
#include <vector>
#include <cxxabi.h>

#include "Matrix.hpp"

using namespace algebra;

namespace utils{

template<typename T>
void print(const std::vector<T>& vec, const std::string& delimiter = ", ");

std::string demangle(const char* mangled);

template<typename T>
std::vector<std::vector<T>> getRandomSparseMatrix(size_t rows, size_t cols, double random_factor, T min_val = T(1), T max_val = T(9));

template<typename T>
std::vector<T> getRandomVector(size_t n, T min_val = T(1), T max_val = T(9));

} // namespace utils

#include "Utils.tpp" // Include implementations

#endif // UTILS_HPP