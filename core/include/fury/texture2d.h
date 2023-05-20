#pragma once

#include <string>
#include <glad/glad.h>

#include "fury/trace.h"

class Texture2d {
    GLuint m_Texture;
public:
    Texture2d(const std::string &name);
    void Begin();
    void End();
    ~Texture2d();
};