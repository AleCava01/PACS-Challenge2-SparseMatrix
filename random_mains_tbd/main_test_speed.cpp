#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace algebra;

int main() {
    std::cout << "=== Start performance comparison test ===\n\n";

    std::vector<size_t> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

    std::cout << std::left << std::setw(12) << "MatrixSize"
              << std::setw(20) << "Compressed Time (ms)"
              << std::setw(24) << "Uncompressed Time (ms)" << "\n";
    std::cout << std::string(56, '-') << "\n";

    for (size_t size : sizes) {
        Matrix<int, StorageOrder::RowMajor> mat(getRandomSparseMatrix(size, size, 0.95));
        std::vector<int> vec = getRandomVector(size);

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
    return 0;
}
