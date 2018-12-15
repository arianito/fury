//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_VEC3_H
#define FURY_PRIMITIVES_VEC3_H


#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Uniform.h"
#include "Fury/Primitives/Vec2.h"
#include "Fury/Math.h"


struct Vec3 : Uniform<3, 1> {

    static const Vec3 One;
    static const Vec3 Zero;
    static const Vec3 Left;
    static const Vec3 Right;
    static const Vec3 Up;
    static const Vec3 Down;
    static const Vec3 Back;
    static const Vec3 Forward;

    Vec3(F32 scale = 0.0f) : Uniform<3, 1>{scale, scale, scale} {}

    Vec3(F32 x, F32 y, F32 z = 0.0f) : Uniform<3, 1>{x, y, z} {}

    Vec3(const Vec2 &a, F32 z = 0.0f) : Uniform<3, 1>{a[0], a[1], z} {}

    Vec3(const Uniform &a) : Uniform(a) {}

    Vec3(const Uniform &&a) noexcept : Uniform{a} {}

    F32 &x();

    F32 x() const;

    F32 &y();

    F32 y() const;

    F32 &z();

    F32 z() const;

    Vec2 &xy();

    Vec2 xy() const;

    Vec2 &yz();

    Vec2 yz() const;


    F32 Dot(const Vec3 &a) const;

    F32 Magnitude() const;

    F32 SqrMagnitude() const;

    Vec3 Normal() const;

    Vec3 Cross(const Vec3 &a) const;


    static const F32 Dot(const Vec3 &a, const Vec3 &b);

    static const F32 Magnitude(const Vec3 &a);

    static const F32 SqrMagnitude(const Vec3 &a);

    static const Vec3 Normalize(const Vec3 &a);

    static const Vec3 Cross(const Vec3 &a, const Vec3 &b);

    static const Vec3 Lerp(const Vec3 &a, const Vec3 &b, const F32 &t);

    static const Vec3 LerpUnclamped(const Vec3 &a, const Vec3 &b, const F32 &t);

    static const Vec3 MoveTowards(const Vec3 &current, const Vec3 &target, const F32 &maxDistanceDelta);

    static const Vec3 SmoothDamp(const Vec3 &current, const Vec3 &target, Vec3 &currentVelocity, const F32 &smoothTime, const F32 &maxSpeed, const F32 &deltaTime);

    static const Vec3 Reflect(const Vec3 &inDirection, const Vec3 &inNormal);

    static const Vec3 Project(const Vec3 &vector, const Vec3 &onNormal);

    static const Vec3 ProjectOnPlane(const Vec3 &vector, const Vec3 &planeNormal);


    static const F32 Angle(const Vec3 &from, const Vec3 &to);

    static const F32 SignedAngle(const Vec3 &from, const Vec3 &to, const Vec3 &axis);

    static const F32 Distance(const Vec3 &a, const Vec3 &b);

    static const Vec3 ClampMagnitude(const Vec3 &vector, const F32 &maxLength);

    static const Vec3 Min(const Vec3 &lhs, const Vec3 &rhs);

    static const Vec3 Max(const Vec3 &lhs, const Vec3 &rhs);


    friend Vec3 operator+(const Vec3 &a, const Vec3 &b) {
        return Vec3{a[0] + b[0], a[1] + b[1], a[2] + b[2]};
    };

    friend Vec3 operator-(const Vec3 &a) {
        return Vec3{-a[0], -a[1], -a[2]};
    };


    friend Vec3 operator-(const Vec3 &a, const Vec3 &b) {
        return Vec3{a[0] - b[0], a[1] - b[1], a[2] - b[2]};
    };

    friend Vec3 operator*(const Vec3 &a, const Vec3 &b) {
        return Vec3{a[0] * b[0], a[1] * b[1], a[2] * b[2]};
    };

    friend Vec3 operator*(const Vec3 &a, const F32 &b) {
        return Vec3{a[0] * b, a[1] * b, a[2] * b};
    };

    friend Vec3 operator*(const F32 &b, const Vec3 &a) {
        return Vec3{a[0] * b, a[1] * b, a[2] * b};
    };

    friend Vec3 operator/(const Vec3 &a, const F32 &b) {
        return Vec3{a[0] / b, a[1] / b, a[2] / b};
    };


};


#endif //GAME_VEC3v b,./