#ifndef FURY_FILE_H
#define FURY_FILE_H

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


struct File {

    static String ReadFile(const String &path) {
        String vscode;
        std::ifstream vstream(path, std::ios::in);
        if (vstream.is_open()) {
            String line = "";
            while (getline(vstream, line))
                vscode += "\n" + line;
            vstream.close();

            return vscode;
        } else {
            Error(Format("Impossible to open ", path, ".vs"));
            throw;
        }
    }
};

#endif