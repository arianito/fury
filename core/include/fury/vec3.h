#pragma once

#include "system.h"
#include "uniform.h"
#include "vec2.h"
#include "mathf.h"


struct Vec3 : Uniform<3, 1> {

    static const Vec3 one;
    static const Vec3 zero;
    static const Vec3 left;
    static const Vec3 right;
    static const Vec3 up;
    static const Vec3 down;
    static const Vec3 back;
    static const Vec3 forward;

    explicit Vec3(f32 scale = 0.0f) : Uniform<3, 1>{scale, scale, scale} {}

    Vec3(f32 x, f32 y, f32 z = 0.0f) : Uniform<3, 1>{x, y, z} {}

    explicit Vec3(const Vec2 &a, f32 z = 0.0f) : Uniform<3, 1>{a[0], a[1], z} {}

    explicit Vec3(const Uniform &a) : Uniform(a) {}

    explicit Vec3(const Uniform &&a) noexcept : Uniform{a} {}

    f32 &x();

    f32 x() const;

    f32 &y();

    f32 y() const;

    f32 &z();

    f32 z() const;

    Vec2 &xy();

    Vec2 xy() const;

    Vec2 &yz();

    Vec2 yz() const;


    f32 dot(const Vec3 &a) const;

    f32 magnitude() const;

    f32 sqrMagnitude() const;

    Vec3 normal() const;

    Vec3 cross(const Vec3 &a) const;


    static const bool equals(const Vec3 &a, const Vec3 &b);

    static const f32 dot(const Vec3 &a, const Vec3 &b);

    static const f32 magnitude(const Vec3 &a);

    static const f32 sqrMagnitude(const Vec3 &a);

    static const Vec3 normalize(const Vec3 &a);

    static const Vec3 cross(const Vec3 &a, const Vec3 &b);

    static const Vec3 lerp(const Vec3 &a, const Vec3 &b, const f32 &t);

    static const Vec3 lerpUnclamped(const Vec3 &a, const Vec3 &b, const f32 &t);

    static const Vec3 moveTowards(const Vec3 &current, const Vec3 &target, const f32 &maxDistanceDelta);

    static const Vec3 smoothDamp(const Vec3 &current, const Vec3 &target, Vec3 &currentVelocity, const f32 &smoothTime,
                                 const f32 &maxSpeed, const f32 &deltaTime);

    static const Vec3 reflect(const Vec3 &inDirection, const Vec3 &inNormal);

    static const Vec3 project(const Vec3 &vector, const Vec3 &onNormal);

    static const Vec3 projectOnPlane(const Vec3 &vector, const Vec3 &planeNormal);


    static const f32 angle(const Vec3 &from, const Vec3 &to);

    static const f32 signedAngle(const Vec3 &from, const Vec3 &to, const Vec3 &axis);

    static const f32 distance(const Vec3 &a, const Vec3 &b);

    static const Vec3 clampMagnitude(const Vec3 &vector, const f32 &maxLength);

    static const Vec3 min(const Vec3 &lhs, const Vec3 &rhs);

    static const Vec3 max(const Vec3 &lhs, const Vec3 &rhs);


    friend Vec3 operator+(const Vec3 &a, const Vec3 &b);

    friend Vec3 operator-(const Vec3 &a);


    friend Vec3 operator-(const Vec3 &a, const Vec3 &b);

    friend Vec3 operator*(const Vec3 &a, const Vec3 &b);

    friend Vec3 operator*(const Vec3 &a, const f32 &b);

    friend Vec3 operator*(const f32 &b, const Vec3 &a);

    friend Vec3 operator/(const Vec3 &a, const f32 &b);

    friend bool operator==(const Vec3 &a, const Vec3 &b);

    friend bool operator!=(const Vec3 &a, const Vec3 &b);


};