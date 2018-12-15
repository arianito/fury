#include "Fury/Primitives/Vec2.h"

#include "Fury/Math.h"

const Vec2 Vec2::One{1.0f, 1.0f};
const Vec2 Vec2::Zero{0.0f, 0.0f};
const Vec2 Vec2::Left{-1.0f, 0.0f};
const Vec2 Vec2::Right{1.0f, 0.0f};
const Vec2 Vec2::Up{0.0f, 1.0f};
const Vec2 Vec2::Down{0.0f, -1.0f};


F32 Vec2::x() const {
    return data[0];
}

F32 &Vec2::x() {
    return data[0];
}

F32 Vec2::y() const {
    return data[1];
}

F32 &Vec2::y() {
    return data[1];
}


F32 Vec2::Dot(const Vec2 &with) const {
    return data[0] * with[0] + data[1] * with[1];
}

F32 Vec2::Magnitude() const {
    return Math::Sqrt(Dot(*this));
}

F32 Vec2::SqrMagnitude() const {
    return Dot(*this);
}

Vec2 Vec2::Normal() const {
    auto b =  Magnitude();
    if(b < Math::KEPSILON)
        return *this / b;
    return Zero;
}

const F32 Vec2::Dot(const Vec2 &a, const Vec2 &b) {
    return a[0] * b[0] + a[1] * b[1];
}

const F32 Vec2::Magnitude(const Vec2 &a) {
    return Math::Sqrt(Dot(a, a));
}

const F32 Vec2::SqrMagnitude(const Vec2 &a) {
    return Dot(a, a);
}

const Vec2 Vec2::Normalize(const Vec2 &value) {
    auto b = value.Magnitude();
    if(b < Math::KEPSILON)
        return value / b;
    return Zero;
}