#ifndef FURY_PATH_H
#define FURY_PATH_H

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

//
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else

#include <unistd.h>

#define GetCurrentDir getcwd
#endif


struct Path {
    static const String runningPath() {
        char cCurrentPath[FILENAME_MAX];
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
            return String();
        }
        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
        return String(cCurrentPath);
    }

    static char separator() {
#ifdef _WIN32
        return '\\';
#else
        return '/';
#endif
    }

    static String const &to_string(String const &s) { return s; }

    template<typename... Args>
    static String join(Args const &... args) {
        String result;
        int unpack[]{0, (result += to_string(args) + separator(), 0)...};
        static_cast<void>(unpack);
        result.pop_back();
        return result;
    }

    template<typename... Args>
    static String local(Args const &... args) {
        return join(runningPath(), args...);
    }
};


#endif