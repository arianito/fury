//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_MATH_H
#define FURY_MATH_H


#include <cmath>
#include "system.h"
#include "vec2.h"


struct Math {
    static constexpr f32 RAD2DEG{57.295779513082321f};
    static constexpr f32 DEG2RAD{0.017453292519943f};
    static constexpr f32 PI{3.141592653589793f};
    static constexpr f32 HALF_PI{1.570796326794896f};
    static constexpr f32 EPSILON{1.19209e-07};
    static constexpr f32 KEPSILON{0.00001F};
    static constexpr f32 KEPSILON_NORMAL{1e-15F};
    static constexpr f32 MIN{1.17549e-38};
    static constexpr f32 MAX{3.40282e+38};


    static f32 pow(const f32 &a, const f32 &n);

    static f32 sqrt(const f32 &of);

    static f32 invSqrt(const f32 &x);

    static f32 sin(const f32 &of);

    static f32 cos(const f32 &of);

    static f32 tan(const f32 &of);

    static f32 ramp(const f32 &of);

    static f32 arcCos(const f32 &of);

    static f32 exp(const f32 &of);

    static f32 arcSin(const f32 &of);

    static f32 arcTan(const f32 &of);

    static f32 arcTan2(const f32 &y, const f32 &x);

    static f32 log(const f32 &x);

    static f32 log2(const f32 &x);

    static f32 log10(const f32 &x);

    static f32 ceil(const f32 &x);

    static f32 floor(const f32 &x);

    static f32 round(const f32 &x);

    static f32 sign(const f32 &x);

    static f32 abs(const f32 &x);

    static f32 clamp(const f32 &value, const f32 &min, const f32 &max);

    static f32 clamp01(const f32 &value);

    static f32 min(const f32 &a, const f32 &b);

    static f32 max(const f32 &a, const f32 &b);

    static f32 lerp(const f32 &a, const f32 &b, const f32 &t);

    static f32 lerpUnclamped(const f32 &a, const f32 &b, const f32 &t);

    static f32 lerpAngle(const f32 &a, const f32 &b, const f32 &t);

    static f32 moveTowards(const f32 &current, const f32 &target, const f32 &maxDelta);

    static f32 moveTowardsAngle(const f32 &current, const f32 &target, const f32 &maxDelta);

    static f32 smoothStep(const f32 &from, const f32 &to, const f32 &t);

    static f32 gamma(const f32 &value, const f32 &absmax, const f32 &gamma);

    static bool approximately(const f32 &a, const f32 &b);

    static f32 repeat(const f32 &t, const f32 &length);

    static f32 pingPong(const f32 &t, const f32 &length);

    static f32 inverseLerp(const f32 &a, const f32 &b, const f32 &value);

    static const f32 deltaAngle(const f32 &current, const f32 &target);

    static f32 smoothDamp(const f32 &current, const f32 &target, f32 &currentVelocity, const f32 &smoothTime, const f32 &maxSpeed, const f32 &deltaTime);

    static f32 smoothDampAngle(const f32 &current, const f32 &target, f32 &currentVelocity, const f32 &smoothTime, const f32 &maxSpeed, const f32 &deltaTime);

    static bool lineIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result);

    static bool lineSegmentIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result);
};

#endif