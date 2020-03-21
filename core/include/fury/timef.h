//
// Created by Aryan on 10/29/18.
//

#ifndef FURY_GRAPHICS_TIME_H
#define FURY_GRAPHICS_TIME_H

#include "system.h"

struct Time {
    static Time *instance();

    f32 elapsedTime{0};
    f32 deltaTime{0};
    f32 smoothDeltaTime{0};
    f32 fixedDeltaTime{1.0f / 60.0f};
private:
    static Time *m_instance;
};

#endif //ORBITAL_GL_TIME_H
