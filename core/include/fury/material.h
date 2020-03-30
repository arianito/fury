#pragma once

#include <string>
#include "vec2.h"
#include "vec3.h"

struct Material {
    std::string m_Name;
    Vec3 m_Ka;
    Vec3 m_Kd;
    Vec3 m_Ks;
    f32 m_Ns;
    f32 m_Ni;
    f32 m_D;
    i32 m_Illuminant;
    std::string m_MapKa;
    std::string m_MapKd;
    std::string m_MapKs;
    std::string m_MapNs;
    std::string m_MapD;
    std::string m_MapBump;
};
