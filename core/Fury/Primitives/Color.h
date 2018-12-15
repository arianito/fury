//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_COLOR_H
#define FURY_PRIMITIVES_COLOR_H

#include "Fury/Primitives/System.h"

#include "Fury/Primitives/Uniform.h"
#include "Fury/Primitives/Vec3.h"


struct Color : Uniform<4, 1> {
    Color(const Vec3 &a, F32 b = 1.0f) : Uniform<4, 1>{a[0], a[1], a[2], b} {}

    Color(F32 r, F32 g, F32 b, F32 a = 1.0f) : Uniform<4, 1>{r, g, b, a} {}

    Color(const Uniform &a) : Uniform(a) {}

    Color(const Uniform &&a) noexcept : Uniform{a} {}

    F32 &r();

    F32 r() const;

    F32 &g();

    F32 g() const;

    F32 &b();

    F32 b() const;

    F32 &a();

    F32 a() const;

    Vec3 &rgb();

    Vec3 rgb() const;
};

#endif //GAME_COLOR_H
