#pragma once

#include "vec3.h"

struct Ray {

    Ray(const Vec3 &origin, const Vec3 &direction);

    Vec3 getPoint(float distance);

    const Vec3 &origin() const;

    const Vec3 &direction() const;

private:
    Vec3 m_Origin;
    Vec3 m_Direction;
};