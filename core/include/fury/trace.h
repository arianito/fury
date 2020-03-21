#ifndef FURY_TRACE_H
#define FURY_TRACE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <sstream>
#include <memory>
#include <cassert>

#include "system.h"

template<class T>
auto operator<<(std::ostream &os, const T &t) -> decltype(t.print(os), os) {
    t.print(os);
    return os;
}


template<typename... Args>
void log_trace(Args const &... args) {
    int unpack[]{0, (std::cout << args << std::endl, 0)...};
}


template<typename... Args>
void log_info(const char * __format, Args const &... args) {
    fprintf(stdout, __format, args...);
}
template<typename... Args>
void log_error(const char * __format, Args const &... args) {
    fprintf(stderr, __format, args...);
}

template<typename... Args>
void log_fatal(const char * __format, Args const &... args) {
    fprintf(stderr, __format, args...);
    glfwTerminate();
    exit(-1);
}

template<typename... Args>
String join_string(Args const &... args) {
    std::ostringstream os;
    int unpack[]{0, (os << args, 0)...};
    return os.str();
}

#endif