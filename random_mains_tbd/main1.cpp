#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

std::vector<std::vector<int>> getTestMatrix(){
    std::vector<std::vector<int>> matrix(3, std::vector<int>(3, 0));
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][1] = 1;
    matrix[2][0] = 1;
    matrix[2][2] = 1;
    return matrix;
}

int main() {
    // 用固定数据创建稀疏矩阵（未压缩状态）
    Matrix<int, StorageOrder::RowMajor> mat(getTestMatrix());

    // 定义测试向量
    std::vector<int> v = {1, 2, 1};  // 3x1 向量

    // 计算乘法结果（未压缩状态）
    std::vector<int> result = mat.product_by_vector(v);

    // 输出结果
    std::cout << "Result of mat * v:" << std::endl;
    print(result);  // 应该是 [5, 2, 2]

    return 0;
}
