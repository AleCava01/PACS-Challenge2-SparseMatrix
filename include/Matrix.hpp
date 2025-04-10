#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include "StorageOrder.hpp"

namespace algebra {

template<typename T, StorageOrder Order>
class Matrix {

private:
    size_t rows_, cols_;
    std::vector<T> data_;
    size_t index(const size_t i, const size_t j) const;

public:
    Matrix(size_t rows, size_t cols); // Constructor
    void print() const; // Print matrix function
    void info() const; // Prints some useful matrix info
    bool update(const size_t i, const size_t j,const T& value);
};


} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP