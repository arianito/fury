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


struct File {

    static std::string read(const std::string &path) {
        std::ifstream vstream(path, std::ios::in);
        if (vstream.is_open()) {
            std::string vscode;
            std::string line;
            while (vstream.good()) {
                getline(vstream, line);
                vscode.append(line + "\n");
            }
            vstream.close();
            return vscode;
        } else {
            log_fatal("Impossible to open %s", path.c_str());
            throw;
        }
    }
};
