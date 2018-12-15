//
// Created by Aryan on 10/30/18.
//

#include "Fury/Math.h"



const F32 Math::Pow(const F32 & a, const F32 & n) {
    return powf(a, n);
}

const F32 Math::Sqrt(const F32 & of) {
    return sqrtf(of);
}

const F32 Math::InvSqrt(const F32 & x) {
    F32 xhalf = 0.5f * x;
    int i = *(int *) &x;
    i = 0x5f375a86 - (i >> 1);
    auto z = *(F32 *) &i;
    z = z * (1.5f - xhalf * z * z);
    return z;
}

const F32 Math::Sin(const F32 & of) {
    return sinf(of);
}

const F32 Math::Cos(const F32 & of) {
    return cosf(of);
}

const F32 Math::Tan(const F32 & of) {
    return tanf(of);
}

const F32 Math::ArcCos(const F32 & of) {
    return acosf(of);
}

const F32 Math::Exp(const F32 & of) {
    return expf(of);
}

const F32 Math::ArcSin(const F32 & of) {
    return asinf(of);
}

const F32 Math::ArcTan(const F32 & of) {
    return atanf(of);
}

const F32 Math::ArcTan2(const F32 & y, const F32 & x) {
    return atan2f(y, x);
}

const F32 Math::Log(const F32 & x) {
    return logf(x);
}

const F32 Math::Log2(const F32 & x) {
    return log2f(x);
}

const F32 Math::Log10(const F32 & x) {
    return log10f(x);
}

const F32 Math::Ceil(const F32 & x) {
    return ceilf(x);
}

const F32 Math::Floor(const F32 & x) {
    return floorf(x);
}

const F32 Math::Round(const F32 & x) {
    return roundf(x);
}

const F32 Math::Sign(const F32 & x) {
    return x >= 0 ? 1 : -1;
}

const F32 Math::Abs(const F32 & x) {
    return fabsf(x);
}

const F32 Math::Clamp(const F32 & value, const F32 & min, const F32 & max) {
    auto z = value;
    if (value < min)
        z = min;
    else if (value > max)
        z = max;
    return z;
}

const F32 Math::Clamp01(const F32 & value) {
    auto z = value;
    if (value < 0.0f)
        z = 0.0f;
    else if (value > 1.0f)
        z = 1.0f;
    return z;
}


const F32 Math::Min(const F32 & a, const F32 & b) { return a < b ? a : b; }

const F32 Math::Max(const F32 & a, const F32 & b) { return a > b ? a : b; }

const F32 Math::Lerp(const F32 & a, const F32 & b, const F32 & t) {
    return a + (b - a) * Clamp01(t);
}

const F32 Math::LerpUnclamped(const F32 & a, const F32 & b, const F32 & t) {
    return a + (b - a) * t;
}

const F32 Math::LerpAngle(const F32 & a, const F32 & b, const F32 & t) {
    F32 delta = Repeat((b - a), 360);
    if (delta > 180)
        delta -= 360;
    return a + delta * Clamp01(t);
}

const F32 Math::MoveTowards(const F32 & current, const F32 & target, const F32 & maxDelta) {
    if (Abs(target - current) <= maxDelta)
        return target;
    return current + Sign(target - current) * maxDelta;
}

const F32 Math::MoveTowardsAngle(const F32 & current, const F32 & target, const F32 & maxDelta) {
    F32 deltaAngle = DeltaAngle(current, target);
    if (-maxDelta < deltaAngle && deltaAngle < maxDelta)
        return target;
    return MoveTowards(current, current + deltaAngle, maxDelta);
}

const F32 Math::SmoothStep(const F32 & from, const F32 & to, const F32 & t) {
    auto t0 = Clamp01(t);
    t0 = -2.0F * t0 * t0 * t0 + 3.0F * t0 * t0;
    return to * t0 + from * (1.0F - t0);
}

const F32 Math::Gamma(const F32 & value, const F32 & absmax, const F32 & gamma) {
    bool negative = false;
    if (value < 0.0F)
        negative = true;
    F32 absval = Abs(value);
    if (absval > absmax)
        return negative ? -absval : absval;

    F32 result = Pow(absval / absmax, gamma) * absmax;
    return negative ? -result : result;
}

const Boolean Math::Approximately(const F32 & a, const F32 & b) {
    return Abs(b - a) < Max(0.000001f * Max(Abs(a), Abs(b)), EPSILON * 8);
}


const F32 Math::Repeat(const F32 & t, const F32 & length) {
    return Clamp(t - Floor(t / length) * length, 0.0f, length);
}

const F32 Math::PingPong(const F32 & t, const F32 & length) {
    auto t0 = Repeat(t, length * 2.0f);
    return length - Abs(t0 - length);
}

const F32 Math::InverseLerp(const F32 & a, const F32 & b, const F32 & value) {
    if (a != b)
        return Clamp01((value - a) / (b - a));
    else
        return 0.0f;
}

const F32 Math::DeltaAngle(const F32 & current, const F32 & target) {
    F32 delta = Repeat((target - current), 360.0f);
    if (delta > 180.0f)
        delta -= 360.0f;
    return delta;
}


const F32 Math::SmoothDamp(const F32 &current, const F32 &target, F32 &currentVelocity, const F32 &smoothTime, const F32 &maxSpeed, const F32 &deltaTime) {
    auto smoothTime0 = Max(0.0001F, smoothTime);
    F32 omega = 2.0F / smoothTime0;
    F32 x = omega * deltaTime;
    F32 exp = 1.0F / (1.0F + x + 0.48F * x * x + 0.235F * x * x * x);
    F32 change = current - target;
    F32 originalTo = target;

    F32 maxChange = maxSpeed * smoothTime0;
    change = Clamp(change, -maxChange, maxChange);
    auto target0 = current - change;
    F32 temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - omega * temp) * exp;
    F32 output = target0 + (change + temp) * exp;

    if (originalTo - current > 0.0F == output > originalTo) {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}


const F32 Math::SmoothDampAngle(const F32 &current, const F32 &target, F32 &currentVelocity, const F32 &smoothTime, const F32 &maxSpeed, const F32 &deltaTime) {
    auto target0 = current + DeltaAngle(current, target);
    return SmoothDamp(current, target0, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

const Boolean Math::LineIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result) {
    F32 bx = p2[0] - p1[0];
    F32 by = p2[1] - p1[1];
    F32 dx = p4[0] - p3[0];
    F32 dy = p4[1] - p3[1];
    F32 bDotDPerp = bx * dy - by * dx;
    if (bDotDPerp == 0) {
        return false;
    }
    F32 cx = p3[0] - p1[0];
    F32 cy = p3[1] - p1[1];
    F32 t = (cx * dy - cy * dx) / bDotDPerp;

    result = {p1[0] + t * bx, p1[1] + t * by};
    return true;
}

const Boolean Math::LineSegmentIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result) {
    F32 bx = p2[0] - p1[0];
    F32 by = p2[1] - p1[1];
    F32 dx = p4[0] - p3[0];
    F32 dy = p4[1] - p3[1];
    F32 bDotDPerp = bx * dy - by * dx;
    if (bDotDPerp == 0) {
        return false;
    }
    F32 cx = p3[0] - p1[0];
    F32 cy = p3[1] - p1[1];
    F32 t = (cx * dy - cy * dx) / bDotDPerp;
    if (t < 0 || t > 1) {
        return false;
    }
    F32 u = (cx * by - cy * bx) / bDotDPerp;
    if (u < 0 || u > 1) {
        return false;
    }
    result = {p1[0] + t * bx, p1[1] + t * by};
    return true;
}