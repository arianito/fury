
#ifndef FURY_GRAPHICS_SHADER_H
#define FURY_GRAPHICS_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Vec2.h"
#include "Fury/Primitives/Vec3.h"
#include "Fury/Primitives/Vec4.h"
#include "Fury/Primitives/Mat4.h"


#include "Fury/Trace.h"
#include "Fury/File.h"


struct Shader {

    Boolean Create(const String &path){

        auto vscode = File::ReadFile(Format(path, ".vertex.glsl"));
        auto fscode = File::ReadFile(Format(path, ".fragment.glsl"));

        GLint status = GL_TRUE;
        char error_msg[1024];
        GLsizei read;

        char const *vspointer = vscode.c_str();
        auto vsp = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsp, 1, &vspointer, nullptr);
        glCompileShader(vsp);
        glGetShaderiv(vsp, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            glGetShaderInfoLog( vsp, 1024, &read, error_msg);
            Error("compile error:", error_msg);
            throw;
        }


        char const *fspointer = fscode.c_str();
        auto fsp = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsp, 1, &fspointer, nullptr);
        glCompileShader(fsp);
        glGetShaderiv( fsp, GL_COMPILE_STATUS, &status );
        if (status != GL_TRUE) {
            glGetShaderInfoLog( fsp, 1024, &read, error_msg);
            Error("compile error:", error_msg);
            throw;
        }
        program = glCreateProgram();
        glAttachShader(program, vsp);
        glAttachShader(program, fsp);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if ( status != GL_TRUE )
        {
            glGetProgramInfoLog( program, 1024, &read, error_msg );
            Error("compile error:", error_msg);
            throw;
        }
        glDetachShader(program, vsp);
        glDetachShader(program, fsp);
        glDeleteShader(vsp);
        glDeleteShader(fsp);
        return TRUE;
    }

    void Begin() { glUseProgram(program); }

    void End() { glUseProgram(0); }

    void Dispose() { glDeleteProgram(program); }


    GLuint GetProgram() const {
        return program;
    }


    void SetParam(const char *name, const I32 &a) {
        glUniform1i(glGetUniformLocation(program, name), a);
    }

    void SetParam(const char *name, const U32 &a) {
        glUniform1ui(glGetUniformLocation(program, name), a);
    }

    void SetParam(const char *name, const F32 &a) {
        glUniform1f(glGetUniformLocation(program, name), a);
    }

    void SetParam(const char *name, const F64 &a) {
        glUniform1f(glGetUniformLocation(program, name), (F32) a);
    }

    void SetParam(const char *name, const Mat4 &a) {
        glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_TRUE, a.data);
    }

    void SetParam(const char *name, const Vec2 &a) {
        glUniform2fv(glGetUniformLocation(program, name), 1, a.data);
    }

    void SetParam(const char *name, const Vec3 &a) {
        glUniform3fv(glGetUniformLocation(program, name), 1, a.data);
    }

    void SetParam(const char *name, const Vec4 &a) {
        glUniform4fv(glGetUniformLocation(program, name), 1, a.data);
    }

private:
    GLuint program;

};


#endif