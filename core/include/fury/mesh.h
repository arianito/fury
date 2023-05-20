#pragma once

#include <string>
#include <glad/glad.h>
#include "fury/trace.h"
#include "fury/vec2.h"
#include "fury/vec3.h"

#include "vertex.h"
#include "material.h"

struct Mesh {
    Mesh() = default;
    inline Mesh(std::vector<Vertex> &v, std::vector<unsigned int> &i) : m_Vertices(v), m_Indices(i) {}
    std::string m_Name;
    std::vector<Vertex> m_Vertices;
    std::vector<u32> m_Indices;
    Material m_Material;
};