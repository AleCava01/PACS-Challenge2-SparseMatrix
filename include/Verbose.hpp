#ifndef VERBOSE_HPP
#define VERBOSE_HPP

#include "Utils.hpp"

namespace verbose{
    void separator(const size_t i){
        std::cout<<std::string(i,'-')<<std::endl;
    }

} // namespace verbose


#endif //VERBOSE_HPP