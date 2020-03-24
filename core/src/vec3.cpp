

#include "fury/vec3.h"
#include "fury/mathf.h"


const Vec3 Vec3::one{1.0f, 1.0f, 1.0f};
const Vec3 Vec3::zero{0.0f, 0.0f, 0.0f};
const Vec3 Vec3::left{-1.0f, 0.0f, 0.0f};
const Vec3 Vec3::right{1.0f, 0.0f, 0.0f};
const Vec3 Vec3::up{0.0f, 1.0f, 0.0f};
const Vec3 Vec3::down{0.0f, -1.0f, 0.0f};
const Vec3 Vec3::back{0.0f, 0.0f, -1.0f};
const Vec3 Vec3::forward{0.0f, 0.0f, 1.0f};


f32 &Vec3::x() {
    return data[0];
}

f32 Vec3::x() const {
    return data[0];
}

f32 &Vec3::y() {
    return data[1];
}

f32 Vec3::y() const {
    return data[1];
}

f32 &Vec3::z() {
    return data[2];
}

f32 Vec3::z() const {
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


f32 Vec3::dot(const Vec3 &a) const {
    return data[0] * a[0] + data[1] * a[1] + data[2] * a[2];
}

f32 Vec3::magnitude() const {
    return Math::sqrt(dot(*this));
}

f32 Vec3::sqrMagnitude() const {
    return dot(*this);
}

Vec3 Vec3::normal() const {
    auto b = this->magnitude();
    if (b > Math::KEPSILON)
        return *this / b;
    return zero;
}

Vec3 Vec3::cross(const Vec3 &a) const {
    return Vec3{data[1] * a[2] - data[2] * a[1],
                data[2] * a[0] - data[0] * a[2],
                data[0] * a[1] - data[1] * a[0]};
}


const f32 Vec3::dot(const Vec3 &a, const Vec3 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

const f32 Vec3::magnitude(const Vec3 &a) {
    return Math::sqrt(dot(a, a));
}

const f32 Vec3::sqrMagnitude(const Vec3 &a) {
    return dot(a, a);
}

const Vec3 Vec3::normalize(const Vec3 &a) {
    auto b = magnitude(a);
    if (b > Math::KEPSILON)
        return a / b;
    return Vec3{0};
}

const Vec3 Vec3::cross(const Vec3 &a, const Vec3 &b) {

    return Vec3{a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]};
}


const Vec3 Vec3::lerp(const Vec3 &a, const Vec3 &b, const float &t) {

    auto t0 = Math::clamp01(t);
    return Vec3{
            a[0] + (b[0] - a[0]) * t0,
            a[1] + (b[1] - a[1]) * t0,
            a[2] + (b[2] - a[2]) * t0
    };
}

const Vec3 Vec3::lerpUnclamped(const Vec3 &a, const Vec3 &b, const float &t) {

    return Vec3{
            a[0] + (b[0] - a[0]) * t,
            a[1] + (b[1] - a[1]) * t,
            a[2] + (b[2] - a[2]) * t
    };
}

const Vec3 Vec3::moveTowards(const Vec3 &current, const Vec3 &target, const f32 &maxDistanceDelta) {

    Vec3 toVector = target - current;
    f32 dist = toVector.magnitude();
    if (dist <= maxDistanceDelta || dist < Math::EPSILON)
        return target;
    return current + toVector / dist * maxDistanceDelta;
}

const Vec3 Vec3::smoothDamp(const Vec3 &current, const Vec3 &target, Vec3 &currentVelocity, const f32 &smoothTime,
                            const f32 &maxSpeed, const f32 &deltaTime) {

    auto smoothTime0 = Math::max(0.0001F, smoothTime);
    f32 omega = 2.0F / smoothTime0;

    f32 x = omega * deltaTime;
    f32 exp = 1.0F / (1.0F + x + 0.48F * x * x + 0.235F * x * x * x);
    Vec3 change = current - target;
    Vec3 originalTo = target;

    f32 maxChange = maxSpeed * smoothTime0;
    change = Vec3::clampMagnitude(change, maxChange);
    Vec3 target0 = current - change;

    Vec3 temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - temp * omega) * exp;
    Vec3 output = target0 + (change + temp) * exp;

    if (Vec3::dot(originalTo - current, output - originalTo) > 0) {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}

const Vec3 Vec3::reflect(const Vec3 &inDirection, const Vec3 &inNormal) {
    return -2.0F * dot(inNormal, inDirection) * inNormal + inDirection;
}

const Vec3 Vec3::project(const Vec3 &vector, const Vec3 &onNormal) {

    float sqrMag = onNormal.sqrMagnitude();
    if (sqrMag < Math::EPSILON)
        return zero;
    else
        return onNormal * dot(vector, onNormal) / sqrMag;
}

const Vec3 Vec3::projectOnPlane(const Vec3 &vector, const Vec3 &planeNormal) {
    return vector - project(vector, planeNormal);
}


const f32 Vec3::angle(const Vec3 &from, const Vec3 &to) {
    float denominator = Math::sqrt(from.sqrMagnitude() * to.sqrMagnitude());
    if (denominator < Math::KEPSILON_NORMAL)
        return 0.0f;
    float d = Math::clamp(dot(from, to) / denominator, -1.0F, 1.0F);
    return Math::arcCos(d);
}

const f32 Vec3::signedAngle(const Vec3 &from, const Vec3 &to, const Vec3 &axis) {
    float unsignedAngle = angle(from, to);
    float sign = Math::sign(dot(axis, cross(from, to)));
    return unsignedAngle * sign;
}

const f32 Vec3::distance(const Vec3 &a, const Vec3 &b) {
    return (a - b).magnitude();
}

const Vec3 Vec3::clampMagnitude(const Vec3 &vector, const f32 &maxLength) {

    if (vector.sqrMagnitude() > maxLength * maxLength)
        return vector.normal() * maxLength;
    return vector;
}

const Vec3 Vec3::min(const Vec3 &lhs, const Vec3 &rhs) {
    return Vec3{
            Math::min(lhs[0], rhs[0]),
            Math::min(lhs[1], rhs[1]),
            Math::min(lhs[2], rhs[2])
    };
}

const Vec3 Vec3::max(const Vec3 &lhs, const Vec3 &rhs) {

    return Vec3{
            Math::max(lhs[0], rhs[0]),
            Math::max(lhs[1], rhs[1]),
            Math::max(lhs[2], rhs[2])
    };

}

Vec3 operator/(const Vec3 &a, const f32 &b) {
    return Vec3{a[0] / b, a[1] / b, a[2] / b};
}

Vec3 operator*(const f32 &b, const Vec3 &a) {
    return Vec3{a[0] * b, a[1] * b, a[2] * b};
}

Vec3 operator*(const Vec3 &a, const f32 &b) {
    return Vec3{a[0] * b, a[1] * b, a[2] * b};
}

Vec3 operator*(const Vec3 &a, const Vec3 &b) {
    return Vec3{a[0] * b[0], a[1] * b[1], a[2] * b[2]};
}

Vec3 operator-(const Vec3 &a, const Vec3 &b) {
    return Vec3{a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

Vec3 operator-(const Vec3 &a) {
    return Vec3{-a[0], -a[1], -a[2]};
}

Vec3 operator+(const Vec3 &a, const Vec3 &b) {
    return Vec3{a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}
