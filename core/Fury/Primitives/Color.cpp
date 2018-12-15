#include "Fury/Primitives/Color.h"


F32 &Color::r() {
    return data[0];
}

F32 Color::r() const {
    return data[0];
}

F32 &Color::g() {
    return data[1];
}

F32 Color::g() const {
    return data[1];
}

F32 &Color::b() {
    return data[2];
}

F32 Color::b() const {
    return data[2];
}

F32 &Color::a() {
    return data[3];
}

F32 Color::a() const {
    return data[3];
}

Vec3 &Color::rgb() {
    return *((Vec3 *) (data));
}
Vec3 Color::rgb() const {
    return Vec3{data[0], data[1], data[2]};
}