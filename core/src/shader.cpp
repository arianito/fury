//
// Created by aryan on 3/21/20.
//

#include "fury/shader.h"

void Shader::fromFile(const String &vs, const String &fs) {
    vscode = File::read(vs);
    fscode = File::read(fs);
}

void Shader::fromSource(const String &vs, const String &fs) {
    vscode = vs;
    fscode = fs;
}

void Shader::create() {
    GLint status = GL_TRUE;
    char error_msg[1024];
    GLsizei read;
    char const *vspointer = vscode.c_str();
    auto vsp = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsp, 1, &vspointer, nullptr);
    glCompileShader(vsp);
    glGetShaderiv(vsp, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(vsp, 1024, &read, error_msg);
        log_fatal("compile error:", error_msg);
    }
    char const *fspointer = fscode.c_str();
    auto fsp = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsp, 1, &fspointer, nullptr);
    glCompileShader(fsp);
    glGetShaderiv(fsp, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(fsp, 1024, &read, error_msg);
        log_fatal("compile error:", error_msg);
    }
    program = glCreateProgram();
    glAttachShader(program, vsp);
    glAttachShader(program, fsp);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        glGetProgramInfoLog(program, 1024, &read, error_msg);
        log_fatal("compile error:", error_msg);
    }
    glDetachShader(program, vsp);
    glDetachShader(program, fsp);
    glDeleteShader(vsp);
    glDeleteShader(fsp);
}

void Shader::dispose() { glDeleteProgram(program); }

void Shader::begin() { glUseProgram(program); }

GLuint Shader::getProgram() {
    return program;
}

void Shader::setParam(const char *name, const u32 &a) {
    glUniform1ui(glGetUniformLocation(program, name), a);
}

void Shader::setParam(const char *name, const i32 &a) {
    glUniform1i(glGetUniformLocation(program, name), a);
}

void Shader::setParam(const char *name, const Color &a) {
    glUniform4fv(glGetUniformLocation(program, name), 1, a.data);
}

void Shader::setParam(const char *name, const Vec4 &a) {
    glUniform4fv(glGetUniformLocation(program, name), 1, a.data);
}

void Shader::setParam(const char *name, const Vec3 &a) {
    glUniform3fv(glGetUniformLocation(program, name), 1, a.data);
}

void Shader::setParam(const char *name, const Vec2 &a) {
    glUniform2fv(glGetUniformLocation(program, name), 1, a.data);
}

void Shader::setParam(const char *name, const Mat4 &a) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_TRUE, a.data);
}

void Shader::setParam(const char *name, const f64 &a) {
    glUniform1f(glGetUniformLocation(program, name), (f32) a);
}

void Shader::setParam(const char *name, const f32 &a) {
    glUniform1f(glGetUniformLocation(program, name), a);
}
