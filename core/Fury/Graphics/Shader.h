
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

        auto vid = glCreateShader(GL_VERTEX_SHADER);
        auto fid = glCreateShader(GL_FRAGMENT_SHADER);

        auto vscode = File::ReadFile(Format(path, ".vertex.glsl"));
        auto fscode = File::ReadFile(Format(path, ".fragment.glsl"));

        GLint result = 0;
        GLint ilog = 0;

        char const *vspointer = vscode.c_str();
        glShaderSource(vid, 1, &vspointer, NULL);
        glCompileShader(vid);


        glGetShaderiv(vid, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vid, GL_INFO_LOG_LENGTH, &ilog);

        if (ilog > 0) {
            std::vector<char> vserror(ilog + 1);
            glGetShaderInfoLog(vid, ilog, NULL, &vserror[0]);
            Error("Vertex Shader Error:", &vserror[0]);
            throw;
        }


        char const *fspointer = fscode.c_str();
        glShaderSource(fid, 1, &fspointer, NULL);
        glCompileShader(fid);


        glGetShaderiv(fid, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fid, GL_INFO_LOG_LENGTH, &ilog);

        if (ilog > 0) {
            std::vector<char> fserror(ilog + 1);
            glGetShaderInfoLog(fid, ilog, NULL, &fserror[0]);
            Error("Fragment Shader Error:", &fserror[0]);
            throw;
        }

        program = glCreateProgram();
        glAttachShader(program, vid);
        glAttachShader(program, fid);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &ilog);
        if (ilog > 0) {
            std::vector<char> perror(ilog + 1);
            glGetProgramInfoLog(program, ilog, NULL, &perror[0]);
            Error("Program Shader Error:", &perror[0]);
            throw;
        }


        glDetachShader(program, vid);
        glDetachShader(program, fid);

        glDeleteShader(vid);
        glDeleteShader(fid);

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