//
// Created by Aryan on 10/30/18.
//

#include "fury/mathf.h"


f32 Math::pow(const f32 &a, const f32 &n) {
    return powf(a, n);
}

f32 Math::sqrt(const f32 &of) {
    return sqrtf(of);
}

f32 Math::invSqrt(const f32 &x) {
    f32 xhalf = 0.5f * x;
    int i = *(int *) &x;
    i = 0x5f375a86 - (i >> 1);
    auto z = *(f32 *) &i;
    z = z * (1.5f - xhalf * z * z);
    return z;
}

f32 Math::sin(const f32 &of) {
    return sinf(of);
}

f32 Math::cos(const f32 &of) {
    return cosf(of);
}

f32 Math::tan(const f32 &of) {
    return tanf(of);
}


f32 Math::ramp(const f32 &of) {
    if (of < 0)
        return 0;
    return of;
}

f32 Math::arcCos(const f32 &of) {
    return acosf(of);
}

f32 Math::exp(const f32 &of) {
    return expf(of);
}

f32 Math::arcSin(const f32 &of) {
    return asinf(of);
}

f32 Math::arcTan(const f32 &of) {
    return atanf(of);
}

f32 Math::arcTan2(const f32 &y, const f32 &x) {
    return atan2f(y, x);
}

f32 Math::log(const f32 &x) {
    return logf(x);
}

f32 Math::log2(const f32 &x) {
    return log2f(x);
}

f32 Math::log10(const f32 &x) {
    return log10f(x);
}

f32 Math::ceil(const f32 &x) {
    return ceilf(x);
}

f32 Math::floor(const f32 &x) {
    return floorf(x);
}

f32 Math::round(const f32 &x) {
    return roundf(x);
}

f32 Math::sign(const f32 &x) {
    return x >= 0 ? 1 : -1;
}

f32 Math::abs(const f32 &x) {
    return fabsf(x);
}

f32 Math::clamp(const f32 &value, const f32 &min, const f32 &max) {
    auto z = value;
    if (value < min)
        z = min;
    else if (value > max)
        z = max;
    return z;
}

f32 Math::clamp01(const f32 &value) {
    auto z = value;
    if (value < 0.0f)
        z = 0.0f;
    else if (value > 1.0f)
        z = 1.0f;
    return z;
}


f32 Math::min(const f32 &a, const f32 &b) { return a < b ? a : b; }

f32 Math::max(const f32 &a, const f32 &b) { return a > b ? a : b; }

f32 Math::lerp(const f32 &a, const f32 &b, const f32 &t) {
    return a + (b - a) * clamp01(t);
}

f32 Math::lerpUnclamped(const f32 &a, const f32 &b, const f32 &t) {
    return a + (b - a) * t;
}

f32 Math::lerpAngle(const f32 &a, const f32 &b, const f32 &t) {
    f32 delta = repeat((b - a), 360);
    if (delta > 180)
        delta -= 360;
    return a + delta * clamp01(t);
}

f32 Math::moveTowards(const f32 &current, const f32 &target, const f32 &maxDelta) {
    if (abs(target - current) <= maxDelta)
        return target;
    return current + sign(target - current) * maxDelta;
}

f32 Math::moveTowardsAngle(const f32 &current, const f32 &target, const f32 &maxDelta) {
    f32 da = deltaAngle(current, target);
    if (-maxDelta < da && da < maxDelta)
        return target;
    return moveTowards(current, current + da, maxDelta);
}

f32 Math::smoothStep(const f32 &from, const f32 &to, const f32 &t) {
    auto t0 = clamp01(t);
    t0 = -2.0F * t0 * t0 * t0 + 3.0F * t0 * t0;
    return to * t0 + from * (1.0F - t0);
}

f32 Math::gamma(const f32 &value, const f32 &absmax, const f32 &gamma) {
    bool negative = false;
    if (value < 0.0F)
        negative = true;
    f32 absval = abs(value);
    if (absval > absmax)
        return negative ? -absval : absval;

    f32 result = pow(absval / absmax, gamma) * absmax;
    return negative ? -result : result;
}

bool Math::approximately(const f32 &a, const f32 &b) {
    return abs(b - a) < max(0.000001f * max(abs(a), abs(b)), EPSILON * 8);
}


f32 Math::repeat(const f32 &t, const f32 &length) {
    return clamp(t - floor(t / length) * length, 0.0f, length);
}

f32 Math::pingPong(const f32 &t, const f32 &length) {
    auto t0 = repeat(t, length * 2.0f);
    return length - abs(t0 - length);
}

f32 Math::inverseLerp(const f32 &a, const f32 &b, const f32 &value) {
    if (a != b)
        return clamp01((value - a) / (b - a));
    else
        return 0.0f;
}

const f32 Math::deltaAngle(const f32 &current, const f32 &target) {
    f32 delta = repeat((target - current), 360.0f);
    if (delta > 180.0f)
        delta -= 360.0f;
    return delta;
}


f32 Math::smoothDamp(const f32 &current, const f32 &target, f32 &currentVelocity, const f32 &smoothTime,
                     const f32 &maxSpeed, const f32 &deltaTime) {
    auto smoothTime0 = max(0.0001F, smoothTime);
    f32 omega = 2.0F / smoothTime0;
    f32 x = omega * deltaTime;
    f32 exp = 1.0F / (1.0F + x + 0.48F * x * x + 0.235F * x * x * x);
    f32 change = current - target;
    f32 originalTo = target;

    f32 maxChange = maxSpeed * smoothTime0;
    change = clamp(change, -maxChange, maxChange);
    auto target0 = current - change;
    f32 temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - omega * temp) * exp;
    f32 output = target0 + (change + temp) * exp;

    if (originalTo - current > 0.0F == output > originalTo) {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}


f32 Math::smoothDampAngle(const f32 &current, const f32 &target, f32 &currentVelocity, const f32 &smoothTime,
                          const f32 &maxSpeed, const f32 &deltaTime) {
    auto target0 = current + deltaAngle(current, target);
    return smoothDamp(current, target0, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

bool Math::lineIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result) {
    f32 bx = p2[0] - p1[0];
    f32 by = p2[1] - p1[1];
    f32 dx = p4[0] - p3[0];
    f32 dy = p4[1] - p3[1];
    f32 bDotDPerp = bx * dy - by * dx;
    if (bDotDPerp == 0) {
        return false;
    }
    f32 cx = p3[0] - p1[0];
    f32 cy = p3[1] - p1[1];
    f32 t = (cx * dy - cy * dx) / bDotDPerp;

    result = {p1[0] + t * bx, p1[1] + t * by};
    return true;
}

bool Math::lineSegmentIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, Vec2 &result) {
    f32 bx = p2[0] - p1[0];
    f32 by = p2[1] - p1[1];
    f32 dx = p4[0] - p3[0];
    f32 dy = p4[1] - p3[1];
    f32 bDotDPerp = bx * dy - by * dx;
    if (bDotDPerp == 0) {
        return false;
    }
    f32 cx = p3[0] - p1[0];
    f32 cy = p3[1] - p1[1];
    f32 t = (cx * dy - cy * dx) / bDotDPerp;
    if (t < 0 || t > 1) {
        return false;
    }
    f32 u = (cx * by - cy * bx) / bDotDPerp;
    if (u < 0 || u > 1) {
        return false;
    }
    result = {p1[0] + t * bx, p1[1] + t * by};
    return true;
}