#include "fury/vec2.h"

#include "fury/mathf.h"

const Vec2 Vec2::one{1.0f, 1.0f};
const Vec2 Vec2::zero{0.0f, 0.0f};
const Vec2 Vec2::left{-1.0f, 0.0f};
const Vec2 Vec2::right{1.0f, 0.0f};
const Vec2 Vec2::up{0.0f, 1.0f};
const Vec2 Vec2::down{0.0f, -1.0f};


f32 Vec2::x() const {
    return data[0];
}

f32 &Vec2::x() {
    return data[0];
}

f32 Vec2::y() const {
    return data[1];
}

f32 &Vec2::y() {
    return data[1];
}


f32 Vec2::dot(const Vec2 &a) const {
    return data[0] * a[0] + data[1] * a[1];
}

f32 Vec2::magnitude() const {
    return Math::sqrt(dot(*this));
}

f32 Vec2::sqrMagnitude() const {
    return dot(*this);
}

Vec2 Vec2::normal() const {
    auto b = magnitude();
    if (b > Math::KEPSILON)
        return *this / b;
    return zero;
}

f32 Vec2::dot(const Vec2 &a, const Vec2 &b) {
    return a[0] * b[0] + a[1] * b[1];
}

f32 Vec2::magnitude(const Vec2 &a) {
    return Math::sqrt(dot(a, a));
}

f32 Vec2::sqrMagnitude(const Vec2 &a) {
    return dot(a, a);
}

Vec2 Vec2::normalize(const Vec2 &a) {
    auto b = a.magnitude();
    if (b < Math::KEPSILON)
        return a / b;
    return zero;
}

Vec2 operator/(const Vec2 &a, const f32 &b) {
    return Vec2{a[0] / b, a[1] / b};
}

Vec2 operator*(const f32 &b, const Vec2 &a) {
    return Vec2{a[0] * b, a[1] * b};
}

Vec2 operator*(const Vec2 &a, const f32 &b) {
    return Vec2{a[0] * b, a[1] * b};
}

Vec2 operator*(const Vec2 &a, const Vec2 &b) {
    return Vec2{a[0] * b[0], a[1] * b[1]};
}

Vec2 operator-(const Vec2 &a, const Vec2 &b) {
    return Vec2{a[0] - b[0], a[1] - b[1]};
}

Vec2 operator-(const Vec2 &a) {
    return Vec2{-a[0], -a[1]};
}

Vec2 operator+(const Vec2 &a, const Vec2 &b) {
    return Vec2{a[0] + b[0], a[1] + b[1]};
}

void Vec2::set(f32 x, f32 y) {
    data[0] = x;
    data[1] = y;
}

Vec2 Vec2::perpendicularClockwise() const {
    return Vec2(data[1], -data[0]);
}

Vec2 Vec2::perpendicularCounterClockwise() const {
    return Vec2(-data[1], data[0]);
}

f32 Vec2::angle(const Vec2 &from, const Vec2 &to) {
    auto dif = (to - from).normal();
    return Math::arcTan2(dif.y(), dif.x());
}

f32 Vec2::distance(const Vec2 &a, const Vec2 &b) {
    return (a - b).magnitude();
}


const Vec2 Vec2::lerp(const Vec2 &a, const Vec2 &b, const float &t) {

    auto t0 = Math::clamp01(t);
    return Vec2{
            a[0] + (b[0] - a[0]) * t0,
            a[1] + (b[1] - a[1]) * t0,
    };
}

const Vec2 Vec2::lerpUnclamped(const Vec2 &a, const Vec2 &b, const float &t) {

    return Vec2{
            a[0] + (b[0] - a[0]) * t,
            a[1] + (b[1] - a[1]) * t,
    };
}