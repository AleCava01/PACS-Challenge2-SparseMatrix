

#include "Tests.hpp"
#include <iostream>

int main() {
    const std::string fname = "assets/lnsp_131.mtx.gz";  // 或者 .mtx，无需改动函数签名
    std::cout << "\n>>> Launching Matrix Market speed test...\n\n";
    tests::matrix_market_speedtest(fname);
    return 0;
}
