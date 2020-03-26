#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "system.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

#include "trace.h"
#include "file.h"
#include "color.h"

struct Shader {

    void FromFile(const std::string &vs, const std::string &fs);
    void FromSource(const std::string &vs, const std::string &fs);

    ~Shader();
    void Create();
    void Begin();
    void End();


    GLuint GetProgramID();


    void SetParameter(const char *name, const i32 &a);

    void SetParameter(const char *name, const u32 &a);

    void SetParameter(const char *name, const f32 &a);

    void SetParameter(const char *name, const f64 &a);

    void SetParameter(const char *name, const Mat4 &a);

    void SetParameter(const char *name, const Vec2 &a);

    void SetParameter(const char *name, const Vec3 &a);

    void SetParameter(const char *name, const Vec4 &a);

    void SetParameter(const char *name, const Color &a);

private:
    GLuint m_ProgramId;
    std::string m_VSSource;
    std::string m_FSSource;
};
