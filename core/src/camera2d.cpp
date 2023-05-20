//
// Created by aryan on 3/21/20.
//

#include "fury/camera2d.h"


void Camera2d::Update() {
    if (Input::KeyPress(KEY_LEFT_SHIFT)) {
        if (Input::MouseDown(MOUSE_BUTTON_LEFT)) {
            m_OldMouse = Convert();
            m_OldPos = m_Position;
        }
        if (Input::MousePress(MOUSE_BUTTON_LEFT)) {
            SetPosition(m_OldPos + (Convert() - m_OldMouse));
        }
        if (Input::MouseDown(MOUSE_BUTTON_RIGHT)) {
            m_OldMouse = Input::GetXY();
            m_OldZoom = m_Zoom;
        }
        if (Input::MousePress(MOUSE_BUTTON_RIGHT)) {
            auto pos = Input::GetXY();
            auto diff = (m_OldMouse.y() - pos.y()) / Display::Height * 2;
            SetZoom(Math::clamp(m_OldZoom * (1 + diff) * (1 + diff), 0.0001, 10));
        }
    }

    if(m_Dirty) {
        UpdateMatrices();
        m_Dirty = false;
    }
}

Vec2 Camera2d::ScreenToWorld() {
    return Convert(m_Projection * m_View, Input::GetXY());
}

Vec2 Camera2d::ScreenToWorld(const Vec2 &p) {
    return Convert(m_Projection * m_View, p);
}

Vec2 Camera2d::Convert() {
    return Convert(m_Projection * Mat4::createScale(Vec3(m_Zoom)), Input::GetXY());
}

Vec2 Camera2d::Convert(const Mat4 &m, const Vec2 &p) {
    auto matInverse = m.invert();
    auto vIn = Vec4();
    vIn[0] = 2.0 * (p.x() / Display::Width) - 1.0f;
    vIn[1] = -2.0 * (p.y() / Display::Height) + 1.0f;
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

void Camera2d::SetPosition(const Vec2 &p) {
    m_Position = p;
    m_Dirty = true;
}

void Camera2d::SetZoom(f32 zoom) {
    m_Zoom = zoom;
    m_Dirty = true;
}

void Camera2d::UpdateMatrices() {
    f32 height = m_Horizon / Display::GetRatio();
    m_Projection = Mat4::orthographic(height / 2, m_Horizon / 2, -height / 2, -m_Horizon / 2, -100, 100);
    m_View = Mat4::createTranslation(Vec3{m_Position.x() * m_Zoom, m_Position.y() * m_Zoom, 10}) *
             Mat4::createScale(Vec3(m_Zoom));
}

const Mat4 &Camera2d::GetView() const {
    return m_View;
}

const Mat4 &Camera2d::GetProjection() const {
    return m_Projection;
}
