

#include "Fury/Primitives/Vec3.h"
#include "Fury/Math.h"


const Vec3 Vec3::One{1.0f, 1.0f, 1.0f};
const Vec3 Vec3::Zero{0.0f, 0.0f, 0.0f};
const Vec3 Vec3::Left{-1.0f, 0.0f, 0.0f};
const Vec3 Vec3::Right{1.0f, 0.0f, 0.0f};
const Vec3 Vec3::Up{0.0f, 1.0f, 0.0f};
const Vec3 Vec3::Down{0.0f, -1.0f, 0.0f};
const Vec3 Vec3::Back{0.0f, 0.0f, -1.0f};
const Vec3 Vec3::Forward{0.0f, 0.0f, 1.0f};


F32 &Vec3::x() {
    return data[0];
}

F32 Vec3::x() const {
    return data[0];
}

F32 &Vec3::y() {
    return data[1];
}

F32 Vec3::y() const {
    return data[1];
}

F32 &Vec3::z() {
    return data[2];
}

F32 Vec3::z() const {
    return data[2];
}

Vec2 &Vec3::xy() {
    return *((Vec2 *) (data));
}

Vec2 Vec3::xy() const {
    return Vec2{data[0], data[1]};
}

Vec2 &Vec3::yz() {
    return *((Vec2 *) (data + 1));
}

Vec2 Vec3::yz() const {
    return Vec2{data[1], data[2]};
}


F32 Vec3::Dot(const Vec3 &with) const {
    return data[0] * with[0] + data[1] * with[1] + data[2] * with[2];
}

F32 Vec3::Magnitude() const {
    return Math::Sqrt(Dot(*this));
}

F32 Vec3::SqrMagnitude() const {
    return Dot(*this);
}

Vec3 Vec3::Normal() const {
    auto b = this->Magnitude();
    if (b > Math::KEPSILON)
        return *this / b;
    return Vec3{0};
}

Vec3 Vec3::Cross(const Vec3 &b) const {
    return Vec3{data[1] * b[2] - data[2] * b[1],
                data[2] * b[0] - data[0] * b[2],
                data[0] * b[1] - data[1] * b[0]};
}


const F32 Vec3::Dot(const Vec3 &a, const Vec3 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

const F32 Vec3::Magnitude(const Vec3 &a) {
    return Math::Sqrt(Dot(a, a));
}

const F32 Vec3::SqrMagnitude(const Vec3 &a) {
    return Dot(a, a);
}

const Vec3 Vec3::Normalize(const Vec3 &value) {
    auto b = Magnitude(value);
    if (b > Math::KEPSILON)
        return value / b;
    return Vec3{0};
}

const Vec3 Vec3::Cross(const Vec3 &a, const Vec3 &b) {

    return Vec3{a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]};
}


const Vec3 Vec3::Lerp(const Vec3 &a, const Vec3 &b, const float &t) {

    auto t0 = Math::Clamp01(t);
    return Vec3{
            a[0] + (b[0] - a[0]) * t0,
            a[1] + (b[1] - a[1]) * t0,
            a[2] + (b[2] - a[2]) * t0
    };
}

const Vec3 Vec3::LerpUnclamped(const Vec3 &a, const Vec3 &b, const float &t) {

    return Vec3{
            a[0] + (b[0] - a[0]) * t,
            a[1] + (b[1] - a[1]) * t,
            a[2] + (b[2] - a[2]) * t
    };
}

const Vec3 Vec3::MoveTowards(const Vec3 &current, const Vec3 &target, const F32 &maxDistanceDelta) {

    Vec3 toVector = target - current;
    F32 dist = toVector.Magnitude();
    if (dist <= maxDistanceDelta || dist < Math::EPSILON)
        return target;
    return current + toVector / dist * maxDistanceDelta;
}

const Vec3 Vec3::SmoothDamp(const Vec3 &current, const Vec3 &target, Vec3 &currentVelocity, const F32 &smoothTime, const F32 &maxSpeed, const F32 &deltaTime) {

    auto smoothTime0 = Math::Max(0.0001F, smoothTime);
    F32 omega = 2.0F / smoothTime0;

    F32 x = omega * deltaTime;
    F32 exp = 1.0F / (1.0F + x + 0.48F * x * x + 0.235F * x * x * x);
    Vec3 change = current - target;
    Vec3 originalTo = target;

    F32 maxChange = maxSpeed * smoothTime0;
    change = Vec3::ClampMagnitude(change, maxChange);
    Vec3 target0 = current - change;

    Vec3 temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - temp * omega) * exp;
    Vec3 output = target0 + (change + temp) * exp;

    if (Vec3::Dot(originalTo - current, output - originalTo) > 0) {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}

const Vec3 Vec3::Reflect(const Vec3 &inDirection, const Vec3 &inNormal) {
    return -2.0F * Dot(inNormal, inDirection) * inNormal + inDirection;
}

const Vec3 Vec3::Project(const Vec3 &vector, const Vec3 &onNormal) {

    float sqrMag = onNormal.SqrMagnitude();
    if (sqrMag < Math::EPSILON)
        return Zero;
    else
        return onNormal * Dot(vector, onNormal) / sqrMag;
}

const Vec3 Vec3::ProjectOnPlane(const Vec3 &vector, const Vec3 &planeNormal) {
    return vector - Project(vector, planeNormal);
}


const F32 Vec3::Angle(const Vec3 &from, const Vec3 &to) {
    float denominator = Math::Sqrt(from.SqrMagnitude() * to.SqrMagnitude());
    if (denominator < Math::KEPSILON_NORMAL)
        return 0.0f;
    float dot = Math::Clamp(Dot(from, to) / denominator, -1.0F, 1.0F);
    return Math::ArcCos(dot) * Math::RAD2DEG;
}

const F32 Vec3::SignedAngle(const Vec3 &from, const Vec3 &to, const Vec3 &axis) {
    float unsignedAngle = Angle(from, to);
    float sign = Math::Sign(Dot(axis, Cross(from, to)));
    return unsignedAngle * sign;
}

const F32 Vec3::Distance(const Vec3 &a, const Vec3 &b) {
    return (a - b).Magnitude();
}

const Vec3 Vec3::ClampMagnitude(const Vec3 &vector, const F32 &maxLength) {

    if (vector.SqrMagnitude() > maxLength * maxLength)
        return vector.Normal() * maxLength;
    return vector;
}

const Vec3 Vec3::Min(const Vec3 &lhs, const Vec3 &rhs) {
    return Vec3{
            Math::Min(lhs[0], rhs[0]),
            Math::Min(lhs[1], rhs[1]),
            Math::Min(lhs[2], rhs[2])
    };
}

const Vec3 Vec3::Max(const Vec3 &lhs, const Vec3 &rhs) {

    return Vec3{
            Math::Max(lhs[0], rhs[0]),
            Math::Max(lhs[1], rhs[1]),
            Math::Max(lhs[2], rhs[2])
    };

}