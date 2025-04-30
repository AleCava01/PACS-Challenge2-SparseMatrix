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

namespace utils {

/**
 * @brief Prints the elements of a vector with a specified delimiter.
 * 
 * This function prints the elements of a given vector of any type `T` with a delimiter 
 * between the elements. By default, the delimiter is a comma followed by a space.
 * 
 * @tparam T The type of elements in the vector (e.g., int, double, std::complex).
 * @param vec The vector whose elements are to be printed.
 * @param delimiter The delimiter to separate the elements (default is ", ").
 */
template<typename T>
void print(const std::vector<T>& vec, const std::string& delimiter = ", ");

/**
 * @brief Demangles a C++ symbol.
 * 
 * This function takes a mangled C++ symbol (as returned by the typeid operator) 
 * and demangles it to its readable form. Useful for debugging or logging type names.
 * 
 * @param mangled The mangled C++ symbol.
 * @return A demangled, human-readable string representation of the symbol.
 */
std::string demangle(const char* mangled);

/**
 * @brief Generates a random sparse matrix of a specified size.
 * 
 * This function generates a sparse matrix of size `rows` × `cols`, where each element
 * is assigned with a probability of `1 - random_factor`. Non-zero elements are assigned
 * random values uniformly distributed between `min_val` and `max_val`.
 * 
 * @tparam T The type of matrix elements (e.g., int, double, std::complex).
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @param random_factor The probability that an element will be zero (sparsity factor).
 * @param min_val The minimum value for non-zero elements.
 * @param max_val The maximum value for non-zero elements.
 * @return A 2D vector representing the sparse matrix.
 */
template<typename T>
std::vector<std::vector<T>> getRandomSparseMatrix(size_t rows, size_t cols, double random_factor, T min_val = T(1), T max_val = T(9));

/**
 * @brief Generates a random vector of a specified size.
 * 
 * This function generates a vector of size `n`, where each element is a random number 
 * uniformly distributed between `min_val` and `max_val`. The type of the vector elements 
 * can be integral (e.g., int), floating-point (e.g., double), or complex (std::complex<double>).
 * 
 * @tparam T The type of vector elements (e.g., int, double, std::complex).
 * @param n The size of the vector.
 * @param min_val The minimum value for elements.
 * @param max_val The maximum value for elements.
 * @return A vector of randomly generated values.
 */
template<typename T>
std::vector<T> getRandomVector(size_t n, T min_val = T(1), T max_val = T(9));

} // namespace utils

#include "Utils.tpp" // Include implementations

#endif // UTILS_HPP
