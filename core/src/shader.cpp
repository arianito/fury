//
// Created by aryan on 3/21/20.
//

#include "fury/shader.h"

void Shader::FromFile(const std::string &vs, const std::string &fs) {
    m_VSSource = File::read(vs);
    m_FSSource = File::read(fs);
}

void Shader::FromSource(const std::string &vs, const std::string &fs) {
    m_VSSource = vs;
    m_FSSource = fs;
}

void Shader::Create() {
    GLint status = GL_TRUE;
    char error_msg[1024];
    GLsizei read;
    char const *vspointer = m_VSSource.c_str();
    auto vsp = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsp, 1, &vspointer, nullptr);
    glCompileShader(vsp);
    glGetShaderiv(vsp, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(vsp, 1024, &read, error_msg);
        log_fatal("compile error:", error_msg);
    }
    char const *fspointer = m_FSSource.c_str();
    auto fsp = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsp, 1, &fspointer, nullptr);
    glCompileShader(fsp);
    glGetShaderiv(fsp, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(fsp, 1024, &read, error_msg);
        log_fatal("compile error:", error_msg);
    }
    m_ProgramId = glCreateProgram();
    glAttachShader(m_ProgramId, vsp);
    glAttachShader(m_ProgramId, fsp);
    glLinkProgram(m_ProgramId);
    glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        glGetProgramInfoLog(m_ProgramId, 1024, &read, error_msg);
        log_fatal("compile error:", error_msg);
    }
    glDetachShader(m_ProgramId, vsp);
    glDetachShader(m_ProgramId, fsp);
    glDeleteShader(vsp);
    glDeleteShader(fsp);
}

Shader::~Shader() { glDeleteProgram(m_ProgramId); }

void Shader::Begin() { glUseProgram(m_ProgramId); }
void Shader::End() { glUseProgram(0); }

GLuint Shader::GetProgramID() {
    return m_ProgramId;
}

void Shader::SetParameter(const char *name, const u32 &a) {
    glUniform1ui(glGetUniformLocation(m_ProgramId, name), a);
}

void Shader::SetParameter(const char *name, const i32 &a) {
    glUniform1i(glGetUniformLocation(m_ProgramId, name), a);
}

void Shader::SetParameter(const char *name, const Color &a) {
    glUniform4fv(glGetUniformLocation(m_ProgramId, name), 1, a.data);
}

void Shader::SetParameter(const char *name, const Vec4 &a) {
    glUniform4fv(glGetUniformLocation(m_ProgramId, name), 1, a.data);
}

void Shader::SetParameter(const char *name, const Vec3 &a) {
    glUniform3fv(glGetUniformLocation(m_ProgramId, name), 1, a.data);
}

void Shader::SetParameter(const char *name, const Vec2 &a) {
    glUniform2fv(glGetUniformLocation(m_ProgramId, name), 1, a.data);
}

void Shader::SetParameter(const char *name, const Mat4 &a) {
    glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name), 1, GL_TRUE, a.data);
}

void Shader::SetParameter(const char *name, const f64 &a) {
    glUniform1f(glGetUniformLocation(m_ProgramId, name), (f32) a);
}

void Shader::SetParameter(const char *name, const f32 &a) {
    glUniform1f(glGetUniformLocation(m_ProgramId, name), a);
}
