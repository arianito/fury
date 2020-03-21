
#ifndef FURY_GRAPHICS_SHADER_H
#define FURY_GRAPHICS_SHADER_H

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
#include "entity.h"

struct Shader: public Entity {

    void fromFile(const String &vs, const String &fs);

    void fromSource(const String &vs, const String &fs);

    void create() override;

    void begin();

    void dispose() override;


    GLuint getProgram();


    void setParam(const char *name, const i32 &a);

    void setParam(const char *name, const u32 &a);

    void setParam(const char *name, const f32 &a);

    void setParam(const char *name, const f64 &a);

    void setParam(const char *name, const Mat4 &a);

    void setParam(const char *name, const Vec2 &a);

    void setParam(const char *name, const Vec3 &a);

    void setParam(const char *name, const Vec4 &a);

    void setParam(const char *name, const Color &a);

private:
    GLuint program;
    String vscode;
    String fscode;
};


#endif