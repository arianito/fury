//
// Created by aryan on 3/21/20.
//

#ifndef GAME_RAY_H
#define GAME_RAY_H

#include "vec3.h"

struct Ray {

    Ray(const Vec3 &origin, const Vec3 &direction);

    Vec3 getPoint(float distance);

    [[nodiscard]] const Vec3 &origin() const;

    [[nodiscard]] const Vec3 &direction() const;

private:
    Vec3 m_Origin;
    Vec3 m_Direction;
};

#endif //GAME_RAY_H
