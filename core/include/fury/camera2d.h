#pragma once

#include "fury/mat4.h"
#include "fury/vec4.h"
#include "fury/game_input.h"
#include "fury/display.h"
#include <cmath>

struct Camera2d {
    void SetPosition(const Vec2 &p);
    void SetZoom(f32 zoom);

    const Mat4& GetView() const;
    const Mat4& GetProjection() const;
    void Update();
    Vec2 ScreenToWorld();
    Vec2 ScreenToWorld(const Vec2 &p);

private:
    Mat4 m_View = Mat4::identify;
    Mat4 m_Projection = Mat4::identify;
    f32 m_Horizon = 15.0f;
    Vec2 m_Position{0, 0};
    f32 m_Zoom = 1;
    bool m_Dirty = true;

    void UpdateMatrices();
    Vec2 Convert();

    Vec2 Convert(const Mat4 &m, const Vec2 &p);

    f32 m_OldZoom = 0;
    Vec2 m_OldMouse{0};
    Vec2 m_OldPos{0};
};