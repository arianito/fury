#pragma once

#include "system.h"
#include "uniform.h"
#include "vec3.h"


struct Color : Uniform<4, 1> {
    Color() : Uniform<4, 1>{1, 1, 1, 1} {}

    Color(const Vec3 &a, f32 b = 1.0f) : Uniform<4, 1>{a[0], a[1], a[2], b} {}

    Color(f32 r, f32 g, f32 b, f32 a = 1.0f) : Uniform<4, 1>{r, g, b, a} {}

    Color(const Uniform &a) : Uniform(a) {}

    Color(const Uniform &&a) noexcept : Uniform{a} {}

    f32 &r();

    f32 r() const;

    f32 &g();

    f32 g() const;

    f32 &b();

    f32 b() const;

    f32 &a();

    f32 a() const;

    Vec3 &rgb();

    Vec3 rgb() const;


    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color white;
    static const Color black;
    static const Color yellow;
};
