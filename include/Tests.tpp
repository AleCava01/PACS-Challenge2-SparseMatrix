#ifndef TESTS_TPP
#define TESTS_TPP


namespace tests{

    // Multiplication testing utilities
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication(const Matrix<T, Order>& mat, const std::vector<T>& v) {
    // Measures the execution time of the matrix-vector multiplication
    // and returns both the result and the duration (in microseconds).

        auto start = std::chrono::high_resolution_clock::now(); // start time
        std::vector<T> multiplication_result = mat.product_by_vector(v);
        auto end = std::chrono::high_resolution_clock::now(); // end time

        // Duration in microseconds
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return std::make_tuple(multiplication_result, duration);
    }
    
    template<typename T, StorageOrder Order>
    std::tuple<std::vector<T>, long long> test_multiplication_mean(const Matrix<T, Order>& mat, const std::vector<T>& v, int repetitions) {
    // Measures the average execution time of matrix-vector multiplication
    // over a specified number of repetitions and returns the last result and the mean duration.

        long long total_duration = 0;
        std::vector<T> last_result;

        for (int i = 0; i < repetitions; ++i) {
            auto [result, duration] = test_multiplication(mat, v);
            total_duration += duration;
            last_result = std::move(result); // store only the last value
        }

        long long mean_duration = total_duration / repetitions;
        return {last_result, mean_duration};
    }

    template<typename T>
    std::vector<std::vector<T>> get_fixed_test_matrix() {
        return {
            {T(1), T(0), T(0), T(0)},
            {T(0), T(0), T(1), T(0)},
            {T(0), T(1), T(0), T(0)},
            {T(0), T(0), T(1), T(3)}
        };
    }

    
    // Speed tests
    void multiplication_compressed_vs_uncompressed_speedtest(){
    // Compares the performance (in milliseconds) of matrix-vector multiplication
    // between compressed and uncompressed sparse matrix representations across various matrix sizes.

        std::cout << "=== Start performance comparison test ===\n\n";

        std::vector<size_t> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

        std::cout << std::left << std::setw(12) << "MatrixSize"
                << std::setw(20) << "Compressed Time (ms)"
                << std::setw(24) << "Uncompressed Time (ms)" << "\n";
        std::cout << std::string(56, '-') << "\n";

        for (size_t size : sizes) {
            Matrix<int, StorageOrder::RowMajor> mat(getRandomSparseMatrix<int>(size, size, 0.95));
            std::vector<int> vec = getRandomVector<int>(size);

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
    }
    
    void multiplication_compressed_parallel_vs_unparallel_speedtest(size_t num_iterations) {
    // Benchmarks the performance of compressed matrix-vector multiplication
    // with and without parallelization across various matrix sizes and iterations.
    // Results are printed to console and saved to "performance_results.csv".

        std::cout << "=== Start performance comparison test ===\n\n";

        std::vector<size_t> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000};
        std::ofstream file("output/performance_results.csv");
        file << "Iteration,MatrixSize,Method,TimeMs\n";

        std::cout << std::left << std::setw(12) << "MatrixSize"
                << std::setw(10) << "Iter"
                << std::setw(30) << "Compressed Time (ms)"
                << std::setw(30) << "Compressed Parallel Time (ms)" << "\n";
        std::cout << std::string(90, '-') << "\n";

        for (size_t size : sizes) {
            for (size_t iter = 0; iter < num_iterations; ++iter) {
                Matrix<int, StorageOrder::RowMajor> mat(getRandomSparseMatrix<int>(size, size, 0.95));
                std::vector<int> vec = getRandomVector<int>(size);
                mat.compress();

                // Compressed
                auto start = std::chrono::high_resolution_clock::now();
                auto result1 = mat.product_by_vector(vec);
                auto end = std::chrono::high_resolution_clock::now();
                double time_compressed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                // Compressed Parallel
                start = std::chrono::high_resolution_clock::now();
                auto result2 = mat.compressed_product_by_vector_parallel(vec);
                end = std::chrono::high_resolution_clock::now();
                double time_parallel = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                // Output to console and file
                std::cout << std::left << std::setw(12) << size
                        << std::setw(10) << iter
                        << std::setw(30) << time_compressed
                        << std::setw(30) << time_parallel << "\n";

                // Write on file
                file << iter << "," << size << ",Unparalleled," << time_compressed << "\n";
                file << iter << "," << size << ",Parallel," << time_parallel << "\n";
            }
        }

        file.close();
        std::cout << "\n=== Done. Results saved to performance_results.csv ===\n";
    }
    
    void multiplication_all_speedtest(size_t num_iterations) {
    // Benchmarks matrix-vector multiplication times across multiple cases.
    // Tests compressed and uncompressed matrices in both RowMajor and ColumnMajor layouts.
    // Results are printed in a formatted table for different matrix sizes and iterations.

        std::cout << "=== Start full performance comparison test ===\n\n";

        std::vector<size_t> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000};

        std::cout << std::left << std::setw(12) << "MatrixSize"
                << std::setw(10) << "Iter"
                << std::setw(25) << "Compressed RowMajor"
                << std::setw(25) << "Compressed ColMajor"
                << std::setw(25) << "Uncompressed RowMajor"
                << std::setw(25) << "Uncompressed ColMajor"
                << "\n";
        std::cout << std::string(150, '-') << "\n";

        for (size_t size : sizes) {
            for (size_t iter = 0; iter < num_iterations; ++iter) {
                // Random matrices and vector
                Matrix<int, StorageOrder::RowMajor> mat_row(getRandomSparseMatrix<int>(size, size, 0.95));
                Matrix<int, StorageOrder::ColumnMajor> mat_col(getRandomSparseMatrix<int>(size, size, 0.95));
                std::vector<int> vec = getRandomVector<int>(size);

                // --- Compressed RowMajor ---
                mat_row.compress();
                auto start = std::chrono::high_resolution_clock::now();
                auto res1 = mat_row.product_by_vector(vec);
                auto end = std::chrono::high_resolution_clock::now();
                double time_compressed_row = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                mat_row.decompress();

                // --- Compressed ColumnMajor ---
                mat_col.compress();
                start = std::chrono::high_resolution_clock::now();
                auto res3 = mat_col.product_by_vector(vec);
                end = std::chrono::high_resolution_clock::now();
                double time_compressed_col = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                mat_col.decompress();

                // --- Uncompressed RowMajor ---
                start = std::chrono::high_resolution_clock::now();
                auto res5 = mat_row.product_by_vector(vec);
                end = std::chrono::high_resolution_clock::now();
                double time_uncompressed_row = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                // --- Uncompressed ColumnMajor ---
                start = std::chrono::high_resolution_clock::now();
                auto res6 = mat_col.product_by_vector(vec);
                end = std::chrono::high_resolution_clock::now();
                double time_uncompressed_col = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                // --- Print the results ---
                std::cout << std::left << std::setw(12) << size
                        << std::setw(10) << iter
                        << std::setw(25) << time_compressed_row
                        << std::setw(25) << time_compressed_col
                        << std::setw(25) << time_uncompressed_row
                        << std::setw(25) << time_uncompressed_col
                        << "\n";
            }
        }

        std::cout << "\n=== Done. Results shown above. ===\n";
    }

    void matrix_market_speedtest(const std::string& filename) {

        std::cout << "=== Matrix Market Speed Test (" << filename << ") ===\n\n";

        // Load sparse matrix from file
        algebra::Matrix<double, StorageOrder::RowMajor> mat(0, 0);
        if (!mat.mm_load_mtx(filename)) {
            std::cerr << "❌ Failed to load Matrix Market file: " << filename << "\n";
            return;
        }

     
        mat.info();

        // Generate a random vector with length equal to the number of columns
        size_t n_cols = mat.size()[0];
        std::vector<double> vec = getRandomVector<double>(n_cols);

        // Compressed
        mat.compress();
        auto [res_c, time_c] = test_multiplication(mat, vec);

        // Uncompressed
        mat.decompress();
        auto [res_u, time_u] = test_multiplication(mat, vec);

        // Print results
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "\nCompressed Time:   " << (time_c / 1000.0) << " ms\n";
        std::cout << "Uncompressed Time: " << (time_u / 1000.0) << " ms\n";
        std::cout << "=== Done ===\n";
    }


    // Simple Matrix * Vector tests
    void matrix_vector_multiplication_test_1() {
    // Tests matrix-vector multiplication on both compressed and uncompressed sparse matrices.
    // Initializes a sparse matrix and a vector, performs compression/decompression, and measures execution time.
        
        const size_t n_row = 5, n_col = 5;
        const double sparsity_coeff = 0.7;

        Matrix<int, algebra::StorageOrder::RowMajor> mat(getRandomSparseMatrix<int>(n_row, n_col, sparsity_coeff));
        std::vector<int> v = getRandomVector<int>(n_row);

        auto run_test = [&](const std::string& phase) {
            std::cout << std::string(50, '-') << std::endl;
            std::cout << phase << std::endl;
            mat.print();
            mat.info();

            std::cout << std::string(50, '-') << std::endl;
            std::cout << "v: " << std::endl;
            print(v);
            std::cout << std::endl;

            auto [multiplication_result, duration] = tests::test_multiplication_mean(mat, v, 100);

            std::cout << "Multiplication result: " << std::endl;
            print(multiplication_result);
            std::cout << std::endl;
            std::cout << "Execution time: " << duration << " µs" << std::endl;
        };

    mat.compress();
    run_test("Compressed Matrix Test");

    mat.decompress();
    run_test("Uncompressed Matrix Test");
}
        
    void complex_matrix_times_vector_test(){
        size_t rows = 4;
        size_t cols = 4;
        auto complex_matrix = getRandomSparseMatrix<std::complex<double>>(rows,cols,0.8);
        auto complex_vector = getRandomVector<std::complex<double>>(rows);
        auto mat = Matrix<std::complex<double>,StorageOrder::RowMajor>(complex_matrix);
        mat.compress();
        mat.print(20);
        auto multiplication_result = mat.product_by_vector(complex_vector);
        std::cout<<std::string(50,'-')<<std::endl;
        std::cout << "v: " << std::endl;
        print(complex_vector);
        std::cout<<std::endl;
        auto [result, duration] = test_multiplication_mean(mat,complex_vector,100);
        std::cout << "Multiplication result: " << std::endl;
        print(multiplication_result);
        std::cout << std::endl;
        std::cout << "Execution time: " << duration << " µs" << std::endl;
        std::cout<<std::string(50,'-')<<std::endl;
    }
    
    
    // Specific methods tests
    template<typename T, StorageOrder Order>
    void matrix_norm_test(bool use_fixed_matrix, size_t rows, size_t cols, double density) {
        Matrix<T, Order> mat(
            use_fixed_matrix 
                ? get_fixed_test_matrix<T>() 
                : getRandomSparseMatrix<T>(rows, cols, density)
        );

        auto compute_and_print_norms = [](Matrix<T, Order>& m, const std::string& label) {
            std::cout<<std::string(50,'-')<<std::endl;
            std::cout << label << " matrix:" << std::endl;
            m.print();
            auto one_norm = m.template norm<NormType::One>();
            auto infinity_norm = m.template norm<NormType::Infinity>();
            auto frobenius_norm = m.template norm<NormType::Frobenius>();

            std::cout<<std::string(50,'-')<<std::endl;
            std::cout << "One norm:        " << one_norm << std::endl;
            std::cout << "Infinity norm:   " << infinity_norm << std::endl;
            std::cout << "Frobenius norm:  " << frobenius_norm << std::endl;
            std::cout<<std::string(50,'-')<<std::endl;
        };

        mat.compress();
        compute_and_print_norms(mat, "Compressed");

        mat.decompress();
        compute_and_print_norms(mat, "Uncompressed");
    }
    
    void matrix_resize_test() {
        std::cout << "\n=== Matrix Resize Test ===\n\n";

        Matrix<int, StorageOrder::RowMajor> mat(3, 3);
        mat.update(0, 0, 1);
        mat.update(1, 1, 2);
        mat.update(2, 2, 3);
        std::cout << "\n--- Before resize ---\n";
        mat.print();
        mat.info();

        mat.resize(4, 5);

        std::cout << "\n--- After resize ---\n";
        mat.print();
        mat.info();
    }

    void diagonal_view_test() {
        std::cout << "\n=== Diagonal View Test ===\n\n";

        // Create a 4x4 matrix
        Matrix<int, StorageOrder::RowMajor> mat(4, 4);
        mat.update(0, 0, 10);
        mat.update(0, 1, 1);
        mat.update(1, 1, 20);
        mat.update(2, 2, 30);
        mat.update(3, 0, 3);
        mat.update(3, 3, 40);

        // Uncompressed case
        std::cout << "\n--- Uncompressed Matrix ---\n";
        mat.print();
        std::vector<int> diag_uncompressed = mat.diagonal_view();
        std::cout << "Diagonal (uncompressed): ";
        print(diag_uncompressed);

        // Compressed case
        mat.compress();
        std::cout << "\n--- Compressed Matrix ---\n";
        mat.print();
        std::vector<int> diag_compressed = mat.diagonal_view();
        std::cout << "Diagonal (compressed): ";
        print(diag_compressed);
    }

}

#endif //TESTS_TPP
