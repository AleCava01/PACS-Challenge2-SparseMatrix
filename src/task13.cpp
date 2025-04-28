#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main() {
    std::cout << "\n=== Diagonal View Test ===\n";

    // 1. 创建一个简单矩阵
    Matrix<int, StorageOrder::RowMajor> mat(4, 4);

    mat.update(0, 0, 10);
    mat.update(0, 1, 1);
    mat.update(1, 1, 20);
    mat.update(2, 2, 30);
    mat.update(3, 0, 3);
    mat.update(3, 3, 40);

    std::cout << "\n--- Uncompressed Matrix ---\n";
    mat.print();
    auto diag_uncompressed = mat.diagonal_view();
    std::cout << "Diagonal (uncompressed): ";
    print(diag_uncompressed);

    // 2. 压缩矩阵后再提取对角线
    mat.compress();
    std::cout << "\n--- Compressed Matrix ---\n";
    mat.print();
    auto diag_compressed = mat.diagonal_view();
    std::cout << "Diagonal (compressed): ";
    print(diag_compressed);

    return 0;
}
