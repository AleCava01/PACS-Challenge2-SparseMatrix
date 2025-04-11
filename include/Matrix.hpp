#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include "StorageOrder.hpp"
#include <map>
#include <array>
#include <iostream>


namespace algebra {

template<typename T, StorageOrder Order>
class Matrix {

private:
    size_t rows_, cols_;
    std::map<std::array<int, 2>, T> sparse_data_; //sparse dynamic storage
    size_t index(const size_t i, const size_t j) const;

public:
    Matrix(size_t rows, size_t cols); // Constructor
    void print() const; // Print matrix
    void info() const; // Prints some useful matrix info
    bool update(const size_t i, const size_t j,const T& value); // Update element at position (i,j) to the specified value
    //bool update(const size_t k, const T& value); // needed? to be decided.
};


} // namespace algebra

#include "Matrix.tpp" // methods implementation

#endif // MATRIX_HPP