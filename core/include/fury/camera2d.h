//
// Created by aryan on 3/20/20.
//

#ifndef GAME_CAMERA2D_H
#define GAME_CAMERA2D_H

#include "fury/entity.h"
#include "fury/mat4.h"
#include "fury/vec4.h"
#include <cmath>

struct Camera2d : public Entity {
    static Camera2d *instance();

    Mat4 view = Mat4::identify;
    Mat4 projection = Mat4::identify;
    f32 horizon = 5.0f;
    Vec2 position{0, 0};
    f32 zoom = 1;

    void lateUpdate() override;

    Vec2 screenToWorld(const Vec2 &p);

private:
    static Camera2d *m_instance;

    Vec2 convert();

    Vec2 convert(const Mat4 &m, const Vec2 &p);

    f32 lastZoom = 0;
    Vec2 lastMouse{0};
    Vec2 lastPos{0};
};

#endif //GAME_CAMERA2D_H
