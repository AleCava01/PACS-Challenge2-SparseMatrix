#ifndef VERBOSE_HPP
#define VERBOSE_HPP

#include "Utils.hpp"

namespace verbose{
    void separator(const size_t i){
        std::cout<<std::string(i,'-')<<std::endl;
    }
    template<typename T>
    void display_mat_times_vector_results(const std::vector<T>& multiplication_result, const auto& duration){
        std::cout << "Multiplication result: " << std::endl;
        print(multiplication_result);
        std::cout << std::endl;
        std::cout << "Execution time: " << duration << " µs" << std::endl;
    }

} // namespace verbose


#endif //VERBOSE_HPP