//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_VEC2_H
#define FURY_PRIMITIVES_VEC2_H

#include "Fury/Primitives/Uniform.h"

struct Vec2 : Uniform<2, 1> {
    static const Vec2 One;
    static const Vec2 Zero;
    static const Vec2 Left;
    static const Vec2 Right;
    static const Vec2 Up;
    static const Vec2 Down;

    Vec2(F32 scale = 0.0f) : Uniform<2, 1>{scale, scale} {}

    Vec2(F32 x, F32 y) : Uniform<2, 1>{x, y} {}

    Vec2(const Uniform &a) : Uniform(a) {}

    Vec2(const Uniform &&a) noexcept : Uniform{a} {}


    F32 &x();

    F32 x() const;

    F32 &y();

    F32 y() const;


    F32 Dot(const Vec2 &a) const;
    F32 Magnitude() const;
    F32 SqrMagnitude() const;
    Vec2 Normal() const;


    static const F32 Dot(const Vec2& a, const Vec2& b);
    static const F32 Magnitude(const Vec2& a);
    static const F32 SqrMagnitude(const Vec2& a);
    static const Vec2 Normalize(const Vec2& a);

    // operators
    friend Vec2 operator+(const Vec2 &a, const Vec2 &b) {
        return Vec2{a[0] + b[0], a[1] + b[1]};
    };

    friend Vec2 operator-(const Vec2 &a) {
        return Vec2{-a[0], -a[1]};
    };

    friend Vec2 operator-(const Vec2 &a, const Vec2 &b) {
        return Vec2{a[0] - b[0], a[1] - b[1]};
    };

    friend Vec2 operator*(const Vec2 &a, const Vec2 &b) {
        return Vec2{a[0] * b[0], a[1] * b[1]};
    };

    friend Vec2 operator*(const Vec2 &a, const F32 &b) {
        return Vec2{a[0] * b, a[1] * b};
    };

    friend Vec2 operator*(const F32 &b, const Vec2 &a) {
        return Vec2{a[0] * b, a[1] * b};
    };

    friend Vec2 operator/(const Vec2 &a, const F32 &b) {
        return Vec2{a[0] / b, a[1] / b};
    };

};


#endif //GAME_VEC2_H
