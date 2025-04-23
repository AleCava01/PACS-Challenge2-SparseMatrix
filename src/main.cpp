#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include "../include/Tests.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){
    tests::matrix_vector_multiplication_test_1();
    Matrix<double, StorageOrder::RowMajor> mat(1,1);
    mat.mm_load_mtx("./assets/lnsp_131.mtx.gz");
    mat.info();
    //tests::multiplication_compressed_parallel_vs_unparallel_speedtest();
    tests::multiplication_compressed_vs_uncompressed_speedtest();
    return 0;
}