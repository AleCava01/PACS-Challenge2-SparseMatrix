[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/HlQKP7Zu)
# Challenge 2: A Sparse Matrix
<p align="center">
  <a href="https://img.shields.io/badge/C%2B%2B-20-blue"><img src="https://img.shields.io/badge/C%2B%2B-20-blue" alt="C++"></a>
  <a href="https://img.shields.io/badge/GNU Make-3.27.6-blue"><img src="https://img.shields.io/badge/GNU Make-4.3-blue" alt="make"></a>
  <a href="https://img.shields.io/badge/clang-18.1.3-blue"><img src="https://img.shields.io/badge/clang-18.1.3-blue" alt="clang"></a>
</p>


# Project Structure and Description
```
challenge2-gasati/
├── src/
│   ├── main.cpp
├── include/
│   ├── Matrix.hpp
│   ├── Matrix.tpp
|   ├── StorageOrder.hpp
|   ├── Utils.hpp
├── assets
├── Makefile
├── run
├── LICENSE
└── README.md
```
### Parallelization
We implemented a matrix-vector multiplication method that automatically selects parallel or sequential execution based on the number of rows in the matrix. If the matrix is compressed and has more rows than a certain threshold (```NROWS_PARALLELIZATON_LIMIT```), the parallel version is used to leverage performance on large data. Otherwise, the sequential version is used, which is typically faster for smaller inputs due to lower overhead.

To determine the optimal threshold, we conducted multiple benchmark runs comparing both methods on matrices of increasing size. 

![parallel_vs_unparalleled](./assets/parallel_vs_unparalleled.png)

Based on these tests, we found the inversion point—where parallel computation begins to outperform the sequential one—to be around 1500 rows, and thus we set ```NROWS_PARALLELIZATON_LIMIT = 1500``` in ```include\Parameters.hpp```.




# Setup (Linux / macOS)
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
