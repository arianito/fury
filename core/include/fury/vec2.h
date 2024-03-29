#pragma once

#include "uniform.h"

struct Vec2 : Uniform<2, 1> {
    static const Vec2 one;
    static const Vec2 zero;
    static const Vec2 left;
    static const Vec2 right;
    static const Vec2 up;
    static const Vec2 down;

    explicit Vec2(f32 scale = 0.0f) : Uniform<2, 1>{scale, scale} {}

    Vec2(f32 x, f32 y) : Uniform<2, 1>{x, y} {}

    explicit Vec2(const Uniform &a) : Uniform(a) {}

    explicit Vec2(const Uniform &&a) noexcept : Uniform{a} {}


    f32 &x();

    f32 x() const;

    f32 &y();

    f32 y() const;


    f32 dot(const Vec2 &a) const;

    f32 magnitude() const;

    f32 sqrMagnitude() const;

    Vec2 normal() const;

    Vec2 perpendicularClockwise() const;

    Vec2 perpendicularCounterClockwise() const;


    void set(f32 x, f32 y);


    static f32 dot(const Vec2 &a, const Vec2 &b);

    static f32 magnitude(const Vec2 &a);

    static f32 sqrMagnitude(const Vec2 &a);

    static Vec2 normalize(const Vec2 &a);

    static f32 angle(const Vec2 &from, const Vec2 &to);

    static f32 distance(const Vec2 &a, const Vec2 &b);
    static const Vec2 lerp(const Vec2 &a, const Vec2 &b, const f32 &t);

    static const Vec2 lerpUnclamped(const Vec2 &a, const Vec2 &b, const f32 &t);


    // operators
    friend Vec2 operator+(const Vec2 &a, const Vec2 &b);;

    friend Vec2 operator-(const Vec2 &a);;

    friend Vec2 operator-(const Vec2 &a, const Vec2 &b);;

    friend Vec2 operator*(const Vec2 &a, const Vec2 &b);;

    friend Vec2 operator*(const Vec2 &a, const f32 &b);;

    friend Vec2 operator*(const f32 &b, const Vec2 &a);;

    friend Vec2 operator/(const Vec2 &a, const f32 &b);;

};
