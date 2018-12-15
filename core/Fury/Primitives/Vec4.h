
//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_VEC4_H
#define FURY_PRIMITIVES_VEC4_H


#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Uniform.h"
#include "Fury/Primitives/Vec2.h"
#include "Fury/Primitives/Vec3.h"


struct Vec4 : Uniform<4, 1> {

    static const Vec4 One;
    static const Vec4 Zero;

    Vec4(F32 scale = 0.0f) : Uniform{scale, scale, scale, scale} {}

    Vec4(F32 x, F32 y, F32 z = 0.0f, F32 w = 0.0f) : Uniform{x, y, z, w} {}

    Vec4(const Vec3 &a, F32 w = 0.0f) : Uniform{a[0], a[1], a[2], w} {}

    Vec4(const Uniform &a) : Uniform(a) {}

    Vec4(const Uniform &&a) noexcept : Uniform{a} {}


    F32 &x();

    F32 x() const;

    F32 &y();

    F32 y() const;

    F32 &z();

    F32 z() const;

    F32 &w();

    F32 w() const;

    Vec2 &xy();

    Vec2 xy() const;

    Vec2 &yz();

    Vec2 yz() const;

    Vec2 &zw();

    Vec2 zw() const;

    Vec3 &xyz();

    Vec3 xyz() const;

    Vec3 &yzw();

    Vec3 yzw() const;


    F32 Dot(const Vec4 &a) const;

    F32 Magnitude() const;

    F32 SqrMagnitude() const;

    Vec4 Normal() const;


    static const F32 Dot(const Vec4 &a, const Vec4 &b);

    static const F32 Magnitude(const Vec4 &a);

    static const F32 SqrMagnitude(const Vec4 &a);

    static const Vec4 Normalize(const Vec4 &a);


    friend Vec4 operator+(const Vec4 &a, const Vec4 &b) {
        return Vec4{a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};
    };

    friend Vec4 operator-(const Vec4 &a) {
        return Vec4{-a[0], -a[1], -a[2], -a[3]};
    };

    friend Vec4 operator-(const Vec4 &a, const Vec4 &b) {
        return Vec4{a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};
    };

    friend Vec4 operator*(const Vec4 &a, const Vec4 &b) {
        return Vec4{a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]};
    };

    friend Vec4 operator*(const Vec4 &a, const F32 &b) {
        return Vec4{a[0] * b, a[1] * b, a[2] * b, a[3] * b};
    };

    friend Vec4 operator*(const F32 &b, const Vec4 &a) {
        return Vec4{a[0] * b, a[1] * b, a[2] * b, a[3] * b};
    };

    friend Vec4 operator/(const Vec4 &a, const F32 &b) {
        return Vec4{a[0] / b, a[1] / b, a[2] / b, a[3] / b};
    };
};


#endif //GAME_VEC4_H
