

#include "fury/vec4.h"
#include "fury/mathf.h"


const Vec4 Vec4::one{1.0f, 1.0f, 1.0f, 1.0f};
const Vec4 Vec4::zero{0.0f, 0.0f, 0.0f, 0.0f};


f32 &Vec4::x() {
    return data[0];
}

f32 Vec4::x() const {
    return data[0];
}

f32 &Vec4::y() {
    return data[1];
}

f32 Vec4::y() const {
    return data[1];
}

f32 &Vec4::z() {
    return data[2];
}

f32 Vec4::z() const {
    return data[2];
}

f32 &Vec4::w() {
    return data[2];
}

f32 Vec4::w() const {
    return data[2];
}

Vec2 &Vec4::xy() {
    return *((Vec2 *) (data));
}

Vec2 Vec4::xy() const {
    return Vec2{data[0], data[1]};
}

Vec2 &Vec4::yz() {
    return *((Vec2 *) (data + 1));
}

Vec2 Vec4::yz() const {
    return Vec2{data[1], data[2]};
}

Vec2 &Vec4::zw() {
    return *((Vec2 *) (data + 2));
}

Vec2 Vec4::zw() const {
    return Vec2{data[2], data[3]};
}

Vec3 &Vec4::xyz() {
    return *((Vec3 *) (data));
}

Vec3 Vec4::xyz() const {
    return Vec3{data[0], data[1], data[2]};
}

Vec3 &Vec4::yzw() {
    return *((Vec3 *) (data + 1));
}

Vec3 Vec4::yzw() const {
    return Vec3{data[1], data[2], data[3]};
}


f32 Vec4::dot(const Vec4 &a) const {
    return data[0] * a[0] + data[1] * a[1] + data[2] * a[2] + data[3] * a[3];
}

f32 Vec4::magnitude() const {
    return Math::sqrt(dot(*this));
}

f32 Vec4::sqrMagnitude() const {
    return dot(*this);
}

Vec4 Vec4::normal() const {
    auto b = this->magnitude();
    if (b > Math::KEPSILON)
        return *this / b;
    return zero;
}


const f32 Vec4::dot(const Vec4 &a, const Vec4 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

const f32 Vec4::magnitude(const Vec4 &a) {
    return Math::sqrt(dot(a, a));
}

const f32 Vec4::sqrMagnitude(const Vec4 &a) {
    return dot(a, a);
}

const Vec4 Vec4::normalize(const Vec4 &a) {
    return a / magnitude(a);
}

Vec4 operator/(const Vec4 &a, const f32 &b) {
    return Vec4{a[0] / b, a[1] / b, a[2] / b, a[3] / b};
}

Vec4 operator*(const f32 &b, const Vec4 &a) {
    return Vec4{a[0] * b, a[1] * b, a[2] * b, a[3] * b};
}

Vec4 operator*(const Vec4 &a, const f32 &b) {
    return Vec4{a[0] * b, a[1] * b, a[2] * b, a[3] * b};
}

Vec4 operator*(const Vec4 &a, const Vec4 &b) {
    return Vec4{a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]};
}

Vec4 operator-(const Vec4 &a, const Vec4 &b) {
    return Vec4{a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};
}

Vec4 operator-(const Vec4 &a) {
    return Vec4{-a[0], -a[1], -a[2], -a[3]};
}

Vec4 operator+(const Vec4 &a, const Vec4 &b) {
    return Vec4{a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};
}
