#include "../include/Matrix.hpp"
#include "../include/Utils.hpp"
#include "../include/Tests.hpp"
#include <iostream>
#include <vector>

using namespace algebra;

/**
 * @brief Main function to execute various tests.
 * 
 * This function presents a menu to the user to select a specific test case to run. 
 * Based on the user's input, it calls the corresponding test function. The available 
 * test cases include matrix-vector multiplication, speed tests for compressed and 
 * uncompressed matrices, matrix norm calculation, and more.
 * 
 * The tests include:
 * 1. Matrix-Vector Multiplication Test 1
 * 2. Compressed Parallel vs Unparallel Speedtest
 * 3. Compressed vs Uncompressed Speedtest
 * 4. Matrix Market Speedtest (with a sample matrix file)
 * 5. Complex Matrix Times Vector Test
 * 6. Matrix Norm Test (int, RowMajor)
 * 7. Matrix Resize Test
 * 8. Diagonal View Test
 * 9. All (Rowmajor/ ColumnMajor , Compressed / Uncompressed) Multiplication Speedtest
 * 
 * The user is prompted to select a test case by entering a number between 1 and 9.
 * 
 * @return 0 on successful execution of the selected test, or 1 if an invalid choice is entered.
 */
int main() {
    // Prompt the user for a test selection
    std::cout << "Select a test to run:\n";
    std::cout << "1. Matrix-Vector Multiplication Test 1\n";
    std::cout << "2. Compressed Parallel vs Unparallel Speedtest\n";
    std::cout << "3. Compressed vs Uncompressed Speedtest\n";
    std::cout << "4. Matrix Market Speedtest (lnsp_131.mtx.gz)\n";
    std::cout << "5. Complex Matrix Times Vector Test\n";
    std::cout << "6. Matrix Norm Test (int, RowMajor)\n";
    std::cout << "7. Matrix Resize Test\n";
    std::cout << "8. Diagonal View Test\n";
    std::cout << "9. All (Rowmajor/ ColumnMajor , Compressed / Uncompressed) Multiplication Speedtest\n";
    std::cout << "Enter your choice (1-9): ";

    // Read user input for test selection
    int choice;
    std::cin >> choice;

    // Execute corresponding test based on the user's choice
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
            // Handle invalid input
            std::cout << "Invalid choice.\n";
            break;
    }

    // Return success
    return 0;
}
