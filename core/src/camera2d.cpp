//
// Created by aryan on 3/21/20.
//

#include "fury/camera2d.h"

Camera2d *Camera2d::m_instance = nullptr;

Camera2d *Camera2d::instance() {
    if (!m_instance)
        m_instance = new Camera2d();
    return m_instance;
}

void Camera2d::lateUpdate() {

    if (input->keyPress(KEY_LEFT_SHIFT)) {
        if (input->mouseDown(MOUSE_BUTTON_LEFT)) {
            lastMouse = convert();
            lastPos = position;
        }
        if (input->mousePress(MOUSE_BUTTON_LEFT)) {
            auto pos = convert();
            position = lastPos + (pos - lastMouse);
        }
        if (input->mouseDown(MOUSE_BUTTON_RIGHT)) {
            lastMouse = input->position();
            lastZoom = zoom;
        }
        if (input->mousePress(MOUSE_BUTTON_RIGHT)) {
            auto pos = input->position();
            auto diff = (lastMouse.y() - pos.y()) / display->height * 2;
            zoom = Math::clamp(lastZoom * (1 + diff) * (1 + diff), 0.0001, 10);
        }
    }

    f32 height = horizon / display->ratio();
    projection = Mat4::orthographic(height / 2, horizon / 2, -height / 2, -horizon / 2, -100, 100);
    view = Mat4::createTranslation(Vec3{position.x(), position.y(), 0} * zoom) *
           Mat4::createScale(Vec3(zoom));
}

Vec2 Camera2d::screenToWorld(const Vec2 &p) {
    return convert(projection * view, p);
}

Vec2 Camera2d::convert() {
    return convert(projection * Mat4::createScale(Vec3(zoom)), input->position());
}

Vec2 Camera2d::convert(const Mat4 &m, const Vec2 &p) {
    auto matInverse = m.invert();
    auto vIn = Vec4();
    vIn[0] = 2.0 * (p.x() / display->width) - 1.0f;
    vIn[1] = -2.0 * (p.y() / display->height) + 1.0f;
    vIn[2] = 1.0;
    vIn[3] = 1.0;
    auto pos = vIn * matInverse;
    pos[3] = 1.0 / pos[3];
    pos[0] *= pos[3];
    pos[1] *= pos[3];
    pos[2] *= pos[3];
    if (std::isnan(pos[0]))
        pos[0] = 0;
    if (std::isnan(pos[1]))
        pos[1] = 0;
    return {pos[0], pos[1]};
}