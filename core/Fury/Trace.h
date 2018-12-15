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

#include "Fury/Primitives/System.h"

template<class T>
auto operator<<(std::ostream &os, const T &t) -> decltype(t.print(os), os) {
    t.print(os);
    return os;
}


template<typename... Args>
void Trace(Args const &... args) {
    int unpack[]{0, (std::cout << args << std::endl, 0)...};
}

template<typename... Args>
void Error(Args const &... args) {
    int unpack[]{0, (std::cout << args << std::endl, 0)...};
    glfwTerminate();
    exit(-1);
}

template<typename... Args>
String Format(Args const &... args) {
    std::ostringstream os;
    int unpack[]{0, (os << args, 0)...};
    return os.str();
}

#endif