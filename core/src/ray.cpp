//
// Created by aryan on 3/21/20.
//
#include "fury/ray.h"

Ray::Ray(const Vec3 &origin, const Vec3 &direction) {
    m_Origin = origin;
    m_Direction = direction.normal();
}

Vec3 Ray::getPoint(float distance) {
    return m_Origin + m_Direction * distance;
}

const Vec3 &Ray::origin() const {
    return m_Origin;
}

const Vec3 &Ray::direction() const {
    return m_Direction;
}