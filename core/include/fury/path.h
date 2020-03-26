#pragma once

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

#include "system.h"

//
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else

#include <unistd.h>

#define GetCurrentDir getcwd
#endif


struct Path {
    static const std::string runningPath() {
        char cCurrentPath[FILENAME_MAX];
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
            return std::string();
        }
        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
        return std::string(cCurrentPath);
    }

    static char separator() {
#ifdef _WIN32
        return '\\';
#else
        return '/';
#endif
    }

    static std::string const &to_string(std::string const &s) { return s; }

    template<typename... Args>
    static std::string join(Args const &... args) {
        std::string result;
        int unpack[]{0, (result += to_string(args) + separator(), 0)...};
        static_cast<void>(unpack);
        result.pop_back();
        return result;
    }

    template<typename... Args>
    static std::string local(Args const &... args) {
        return join(runningPath(), args...);
    }
};