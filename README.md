[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/HlQKP7Zu)
# Challenge 2: A Sparse Matrix
<p align="center">
  <a href="https://it.wikipedia.org/wiki/C%2B%2B"><img src="https://img.shields.io/badge/C%2B%2B-20-yellow" alt="C++"></a>
  <a href="https://www.gnu.org/software/make/"><img src="https://img.shields.io/badge/GNU Make-4.3-blue" alt="make"></a>
  <a href="https://www.mingw-w64.org/"><img src="https://img.shields.io/badge/g++-13.3.0-red" alt="g++"></a>
  <a href="https://www.open-mpi.org/"><img src="https://img.shields.io/badge/Open MPI-4.1.6-red" alt="g++"></a>
  <a href="https://zlib.net/"><img src="https://img.shields.io/badge/zlib-1.3.1-blue" alt="g++"></a>
</p>


## Project Structure and Description
```
challenge2-gasati/
├── src/
│   ├── main.cpp
├── include/
│   ├── Matrix.hpp
│   ├── Matrix.tpp
|   ├── StorageOrder.hpp
|   ├── Utils.hpp
|   ├── Utils.tpp
|   ├── Tests.hpp
|   ├── Tests.tpp
|   ├── Parameters.hpp
|   ├── CompressedMatrix.hpp
|   ├── NormType.hpp
├── assets
├── extras
|   ├── parallel_vs_unparallel_plot.py
├── output
├── Makefile
├── run
├── doxygen_config
├── LICENSE
└── README.md
```
## Matrix Class Overview
The Matrix class is a template class designed to represent a sparse matrix with optional compression. It supports both uncompressed (COO format) and compressed (CSR/CSC format) representations, providing efficient storage and operations for sparse matrices. The class includes various matrix operations such as multiplication, resizing, updating, and compression/decompression, as well as methods for handling Matrix Market files.

### Key Template Parameters:
T: The type of the matrix elements (e.g., int, float).
Order: The storage order of the matrix, which can be RowMajor or ColumnMajor.

### Methods
#### Constructors

- Matrix(size_t rows, size_t cols): Initializes an empty matrix with the specified number of rows and columns (all values are set to 0).

- Matrix(const std::vector<std::vector<T>>& mat): Initializes the matrix from a 2D vector, updating the sparse data.

#### Matrix Modification

- bool update(const size_t i, const size_t j, const T& value): Updates the value at position (i, j). Inserts or updates a value if non-zero, or removes it if zero.

- void resize(size_t new_rows, size_t new_cols): Resizes the matrix, removing elements outside the new bounds.

#### Compression & Decompression

- void compress(): Compresses the matrix from COO to CSR/CSC format, freeing the uncompressed storage.

- void decompress(): Decompresses the matrix from CSR/CSC to COO format.

#### Matrix-Vector Multiplication

- std::vector<T> product_by_vector(const std::vector<T>& v) const: Multiplies the matrix by a vector (supports both compressed and uncompressed matrices).

- std::vector<T> compressed_product_by_vector(const std::vector<T>& v) const: Multiplies the compressed matrix by a vector.

- std::vector<T> compressed_product_by_vector_parallel(const std::vector<T>& v) const: Multiplies the compressed matrix by a vector in parallel using OpenMP.

#### Matrix Operations

- void transpose(): Transposes the matrix, swapping rows and columns, and recompresses it if necessary.

- std::vector<T> diagonal_view() const: Extracts the diagonal elements of the matrix.

#### Norm Calculation

- template<NormType norm_type> T norm(): Computes a matrix norm (e.g., One, Infinity, or Frobenius) based on the chosen norm type.

#### File Operations

- bool mm_load_mtx(const std::string& filename): Loads a Matrix Market file (.mtx or .mtx.gz) into the matrix's sparse data structure.

#### Information & Printing

- bool is_compressed() const: Checks if the matrix is currently compressed.

- size_t weight() const: Returns the memory usage of the matrix in bytes.

- void print(int width = 6) const: Prints the matrix contents with specified field width.

- void info() const: Prints general information about the matrix (size, storage type, etc.).

### Matrix Storage Formats
- COO (Coordinate Format): The uncompressed storage format where non-zero values are stored as triplets of (row, column, value).

- CSR/CSC (Compressed Sparse Row/Column): The compressed format storing the matrix in arrays of values, row/column indices, and pointers (based on storage order).

This class is ideal for handling large sparse matrices efficiently, with the ability to switch between uncompressed and compressed formats as needed.
### Parallelization
We implemented a matrix-vector multiplication method that automatically selects between parallel and sequential execution based on the number of rows in the matrix, specifically in the case of the CSR storage format. When the matrix is compressed and contains more rows than a predefined threshold (```NROWS_PARALLELIZATON_LIMIT```), the parallel version is employed to enhance performance on larger datasets. Otherwise, the sequential version is preferred, as it tends to be faster for smaller inputs due to reduced overhead.

To determine the optimal threshold, we conducted a series of benchmark experiments comparing both execution strategies on matrices of increasing size.

![parallel_vs_unparalleled_rowmajor](./assets/parallel_vs_unparallel_RowMajor.png)

The results indicated an inversion point at approximately 1000 rows, beyond which parallel computation consistently outperformed the sequential approach. Consequently, we set ```NROWS_PARALLELIZATON_LIMIT = 1000``` in include\Parameters.hpp.

A similar analysis was performed for the CSC storage format, and the same threshold value (```1000```) was adopted.


## Setup (Linux / macOS)
### Prerequisites
Ensure you have the following dependencies installed on your environment:
- C++20-compatible compiler (e.g., GCC, Clang)
- GNU Make
- CMake
- zlib development libraries
- OpenMPI

📦 Install all prerequisites (Debian/Ubuntu)
```
sudo apt update && sudo apt install -y g++ clang cmake make zlib1g-dev libopenmpi-dev openmpi-bin
```
🍎 For macOS (via Homebrew)
```
brew install gcc clang cmake make zlib open-mpi
```

### Building the Project
Clone the repository:
``` 
git clone https://github.com/PACS-24-25/challenge2-gasati.git
cd challenge2-gasati
```
Build with the Makefile:
```
make
```
Run the program:
```
./main
```
