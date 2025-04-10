#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <cxxabi.h>


// Function to get the type name from
std::string demangle(const char* mangled) {
    int status = 0;
    char* realname = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
    std::string result = (status == 0 && realname) ? realname : mangled;
    free(realname);
    return result;
}

#endif // UTILS_HPP