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
#include "../include/Matrix.hpp"


// General vector printer
template<typename T>
void print(const std::vector<T>& vec, const std::string& delimiter = ", "){
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size())
            std::cout << delimiter;
    }
    std::cout << "]" << std::endl;
}

namespace verbose{
    void separator(const size_t i){
        std::cout<<std::string(i,'-')<<std::endl;
    }
    template<typename T>
    void display_mat_times_vector_results(const std::vector<T>& multiplication_result, const auto& duration){
        std::cout << "Multiplication result: " << std::endl;
        print(multiplication_result);
        std::cout << std::endl;
        std::cout << "Execution time: " << duration << " µs" << std::endl;
    }

} // namespace verbose

using namespace algebra;

std::string demangle(const char* mangled) {
// Converts a mangled C++ type name into a human-readable string.
// Takes a const char* representing the mangled name (usually from typeid().name()).
// Returns a std::string with the demangled, readable type name.

    int status = 0;
    char* realname = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
    std::string result = (status == 0 && realname) ? realname : mangled;
    free(realname);
    return result;
}

template<typename T>
std::vector<std::vector<T>> getRandomSparseMatrix(size_t rows, size_t cols, double random_factor, T min_val = T(1), T max_val = T(9)) {
// Generates a sparse matrix of size rows × cols with random elements of type T.
// Each element is set with probability (1 - random_factor), sampled in [min_val, max_val].
// Returns a std::vector<std::vector<T>> representing the sparse matrix.

    std::vector<std::vector<T>> matrix(rows, std::vector<T>(cols, T(0)));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);

    if constexpr (std::is_integral<T>::value) {
        std::uniform_int_distribution<T> val_dist(min_val, max_val);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (prob_dist(gen) > random_factor) {
                    matrix[i][j] = val_dist(gen);
                }
            }
        }
    } else if constexpr (std::is_floating_point<T>::value) {
        std::uniform_real_distribution<T> val_dist(min_val, max_val);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (prob_dist(gen) > random_factor) {
                    matrix[i][j] = val_dist(gen);
                }
            }
        }
    } else if constexpr (std::is_same<T, std::complex<double>>::value) {
        std::uniform_real_distribution<double> val_dist(std::real(min_val), std::real(max_val));
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (prob_dist(gen) > random_factor) {
                    double real = val_dist(gen);
                    double imag = val_dist(gen);
                    matrix[i][j] = std::complex<double>(real, imag);
                }
            }
        }
    } else {
        static_assert(sizeof(T) == 0, "Unsupported type for getRandomSparseMatrix");
    }

    return matrix;
}

template<typename T>
std::vector<T> getRandomVector(size_t n, T min_val = T(1), T max_val = T(9)) {
// Generates a vector of size n with random elements of type T (int, double or std::complex<double>).
// Values are sampled uniformly in the range [min_val, max_val].
// Returns a std::vector<T> containing the generated random values.
    std::vector<T> vec(n, T(0));

    std::random_device rd;
    std::mt19937 gen(rd());

    if constexpr (std::is_integral<T>::value) {
        std::uniform_int_distribution<T> val_dist(min_val, max_val);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = val_dist(gen);
        }
    } else if constexpr (std::is_floating_point<T>::value) {
        std::uniform_real_distribution<T> val_dist(min_val, max_val);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = val_dist(gen);
        }
    } else if constexpr (std::is_same<T, std::complex<double>>::value) {
        std::uniform_real_distribution<double> val_dist(std::real(min_val), std::real(max_val));
        for (size_t i = 0; i < n; ++i) {
            double real = val_dist(gen);
            double imag = val_dist(gen);
            vec[i] = std::complex<double>(real, imag);
        }
    } else {
        static_assert(sizeof(T) == 0, "Unsupported type for getRandomVector");
    }

    return vec;
}


// ✝️ GRAVEYARD : DEPRECATED FUNCTIONS

/* // Overload operator * for vectors: computes the vector dot product.
template<typename T>
T operator*(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector sizes must match for dot product");
    }
    T result = T{};
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }
    return result;
}
 */

#endif // UTILS_HPP