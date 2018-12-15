

#include "Fury/Primitives/Vec4.h"
#include "Fury/Math.h"


const Vec4 Vec4::One{1.0f, 1.0f, 1.0f, 1.0f};
const Vec4 Vec4::Zero{0.0f, 0.0f, 0.0f, 0.0f};




F32 &Vec4::x() {
    return data[0];
}

F32 Vec4::x() const {
    return data[0];
}

F32 &Vec4::y() {
    return data[1];
}

F32 Vec4::y() const {
    return data[1];
}

F32 &Vec4::z() {
    return data[2];
}

F32 Vec4::z() const {
    return data[2];
}

F32 &Vec4::w() {
    return data[2];
}

F32 Vec4::w() const {
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



F32 Vec4::Dot(const Vec4 &with) const {
    return data[0] * with[0] + data[1] * with[1] + data[2] * with[2]+ data[3] * with[3];
}

F32 Vec4::Magnitude() const {
    return Math::Sqrt(Dot(*this));
}

F32 Vec4::SqrMagnitude() const {
    return Dot(*this);
}

Vec4 Vec4::Normal() const {
    return *this / Magnitude();
}


const F32 Vec4::Dot(const Vec4 &a, const Vec4 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]+ a[3] * b[3];
}

const F32 Vec4::Magnitude(const Vec4 &a) {
    return Math::Sqrt(Dot(a, a));
}

const F32 Vec4::SqrMagnitude(const Vec4 &a) {
    return Dot(a, a);
}

const Vec4 Vec4::Normalize(const Vec4 &value) {
    return value / Magnitude(value);
}