// This header file defines parameters used throughout the program.

// Inside the `params` namespace, two constants are defined:
// 1. `NROWS_PARALLELIZATION_LIMIT`: Defines the row limit for parallelization. When the number of rows exceeds this value, parallel processing is applied.
// 2. `BUFFER_SIZE`: Specifies the buffer size used in operations such as reading from files (used in gz files decompression).

#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

namespace params{
    const int NROWS_PARALLELIZATON_LIMIT = 1500;
    constexpr int BUFFER_SIZE = 8192;
}

#endif