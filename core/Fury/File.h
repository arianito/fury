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
        std::ifstream vstream(path, std::ios::in);
        if (vstream.is_open()) {
            String vscode;
            String line;
            while (vstream.good()) {
                getline(vstream, line);
                vscode.append(line + "\n");
            }
            vstream.close();
            return vscode;
        } else {
            Error(Format("Impossible to open ", path, ".vs"));
            throw;
        }
    }
};

#endif