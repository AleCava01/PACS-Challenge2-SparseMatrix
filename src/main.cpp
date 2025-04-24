#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include "../include/Tests.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){
    //tests::matrix_vector_multiplication_test_1();
    //tests::multiplication_compressed_parallel_vs_unparallel_speedtest();
    //tests::multiplication_compressed_vs_uncompressed_speedtest();
    //tests::matrix_market_speedtest("./assets/lnsp_131.mtx.gz");
    tests::complex_matrix_times_vector_test();
    return 0;
}