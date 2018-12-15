//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_MATH_H
#define FURY_MATH_H


#include <cmath>
#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Vec2.h"


struct Math {
    static constexpr F32 RAD2DEG{57.295779513082321f};
    static constexpr F32 DEG2RAD{0.017453292519943f};
    static constexpr F32 PI{3.141592653589793f};
    static constexpr F32 HALF_PI{1.570796326794896f};
    static constexpr F32 EPSILON{1.19209e-07};
    static constexpr F32 KEPSILON{0.00001F};
    static constexpr F32 KEPSILON_NORMAL{1e-15F};
    static constexpr F32 MIN{1.17549e-38};
    static constexpr F32 MAX{3.40282e+38};


    static const F32 Pow(const F32 &a, const F32 &n);

    static const F32 Sqrt(const F32 &of);

    static const F32 InvSqrt(const F32 &x);

    static const F32 Sin(const F32 &of);

    static const F32 Cos(const F32 &of);

    static const F32 Tan(const F32 &of);

    static const F32 ArcCos(const F32 &of);

    static const F32 Exp(const F32 &of);

    static const F32 ArcSin(const F32 &of);

    static const F32 ArcTan(const F32 &of);

    static const F32 ArcTan2(const F32 &y, const F32 &x);

    static const F32 Log(const F32 &x);

    static const F32 Log2(const F32 &x);

    static const F32 Log10(const F32 &x);

    static const F32 Ceil(const F32 &x);

    static const F32 Floor(const F32 &x);

    static const F32 Round(const F32 &x);

    static const F32 Sign(const F32 &x);

    static const F32 Abs(const F32 &x);

    static const F32 Clamp(const F32 &value, const F32 &min, const F32 &max);

    static const F32 Clamp01(const F32 &value);

    static const F32 Min(const F32 &a, const F32 &b);

    static const F32 Max(const F32 &a, const F32 &b);

    static const F32 Lerp(const F32 &a, const F32 &b, const F32 &t);

    static const F32 LerpUnclamped(const F32 &a, const F32 &b, const F32 &t);

    static const F32 LerpAngle(const F32 &a, const F32 &b, const F32 &t);

    static const F32 MoveTowards(const F32 &current, const F32 &target, const F32 &maxDelta);

    static const F32 MoveTowardsAngle(const F32 &current, const F32 &target, const F32 &maxDelta);

    static const F32 SmoothStep(const F32 &from, const F32 &to, const F32 &t);

    static const F32 Gamma(const F32 &value, const F32 &absmax, const F32 &gamma);

    static const Boolean Approximately(const F32 &a, const F32 &b);

    static const F32 Repeat(const F32 &t, const F32 &length);

    static const F32 PingPong(const F32 &t, const F32 &length);

    static const F32 InverseLerp(const F32 &a, const F32 &b, const F32 &value);

    static const F32 DeltaAngle(const F32 &current, const F32 &target);

    static const F32 SmoothDamp(const F32 &current, const F32 &target, F32 &currentVelocity, const F32 &smoothTime, const F32 &maxSpeed, const F32 &deltaTime);

    static const F32 SmoothDampAngle(const F32 &current, const F32 &target, F32 &currentVelocity, const F32 &smoothTime, const F32 &maxSpeed, const F32 &deltaTime);

    static const Boolean LineIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result);

    static const Boolean LineSegmentIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result);
};

#endif