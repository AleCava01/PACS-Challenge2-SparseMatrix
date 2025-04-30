#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include "../include/Tests.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

int main(){
    std::cout << "Select a test to run:\n";
    std::cout << "1. Matrix-Vector Multiplication Test 1\n";
    std::cout << "2. Compressed Parallel vs Unparallel Speedtest\n";
    std::cout << "3. Compressed vs Uncompressed Speedtest\n";
    std::cout << "4. Matrix Market Speedtest (lnsp_131.mtx.gz)\n";
    std::cout << "5. Complex Matrix Times Vector Test\n";
    std::cout << "6. Matrix Norm Test (int, RowMajor)\n";
    std::cout << "7. Matrix Resize Test\n";
    std::cout << "8. Diagonal View Test\n";
    std::cout << "9. Full Multiplication Speedtest\n";
    std::cout << "Enter your choice (1-9): ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            tests::matrix_vector_multiplication_test_1();
            break;
        case 2:
            tests::multiplication_compressed_parallel_vs_unparallel_speedtest();
            break;
        case 3:
            tests::multiplication_compressed_vs_uncompressed_speedtest();
            break;
        case 4:
            tests::matrix_market_speedtest("./assets/lnsp_131.mtx.gz");
            break;
        case 5:
            tests::complex_matrix_times_vector_test();
            break;
        case 6:
            tests::matrix_norm_test<int, StorageOrder::RowMajor>();
            break;
        case 7:
            tests::matrix_resize_test();
            break;
        case 8:
            tests::diagonal_view_test();
            break;
        case 9:
            tests::multiplication_all_speedtest();
            break;
        default:
            std::cout << "Invalid choice.\n";
            break;
    }

    return 0;
}