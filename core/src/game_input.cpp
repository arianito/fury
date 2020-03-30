//
// Created by aryan on 3/21/20.
//

#include "fury/game_input.h"

GLFWwindow *Input::m_Window = nullptr;

u8 Input::m_KeyboardState[350];
u8 Input::m_OldKeyboardState[350];
u8 Input::m_MouseState[8];
u8 Input::m_OldMouseState[8];
float Input::m_Axises[4];
float Input::m_AxisVelocities[4];
double Input::m_XPos = 0;
double Input::m_YPos = 0;
double Input::m_oldXPos = 0;
double Input::m_oldYPos = 0;

void Input::MoveAxis(u16 ax, bool low, bool high) {
    auto delta = Time::DeltaTime;
    float to = 0.0f + (low ? -1.0f : 0.0f) + (high ? 1.0f : 0.0f);
    if (fabsf(m_Axises[ax] - to) > Math::KEPSILON) {
        m_Axises[ax] = Math::smoothDamp(m_Axises[ax], to, m_AxisVelocities[ax], 0.2f, 40, delta);
    }

}

void Input::Update() {
    for (int i = 0; i < 350; i++)
        m_OldKeyboardState[i] = m_KeyboardState[i];
    for (int i = 0; i < 8; i++)
        m_OldMouseState[i] = m_MouseState[i];
    MoveAxis(AXIS_HORIZONTAL, KeyPress(KEY_LEFT) || KeyPress(KEY_A), KeyPress(KEY_RIGHT) || KeyPress(KEY_D));
    MoveAxis(AXIS_VERTICAL, KeyPress(KEY_DOWN) || KeyPress(KEY_S), KeyPress(KEY_UP) || KeyPress(KEY_W));

    glfwGetCursorPos(m_Window, &m_XPos, &m_YPos);
    m_Axises[AXIS_MOUSE_Y] = Math::smoothDamp(m_Axises[AXIS_MOUSE_Y], m_oldYPos - m_YPos, m_AxisVelocities[AXIS_MOUSE_Y], 0.2f, 40, Time::DeltaTime);
    m_Axises[AXIS_MOUSE_X] = Math::smoothDamp(m_Axises[AXIS_MOUSE_X], m_XPos - m_oldXPos, m_AxisVelocities[AXIS_MOUSE_X], 0.2f, 40, Time::DeltaTime);
    m_oldXPos = m_XPos;
    m_oldYPos = m_YPos;
}

float Input::GetAxis(u8 a) {
    return m_Axises[a];
}

Vec2 Input::GetXY() {
    return {(f32) m_XPos, (f32) m_YPos};
}

Vec3 Input::GetXYZ() {
    return {(f32) m_XPos, (f32) m_YPos, 0.0f};
}

f32 Input::GetX() {
    return m_XPos;
}

f32 Input::GetY() {
    return m_YPos;
}

bool Input::MousePress(u16 key) {
    m_MouseState[key] = glfwGetMouseButton(m_Window, key);
    return m_MouseState[key] != 0;
}

bool Input::MouseUp(u16 key) {
    m_MouseState[key] = glfwGetMouseButton(m_Window, key);
    return !m_MouseState[key] && m_OldMouseState[key];
}

bool Input::MouseDown(u16 key) {
    m_MouseState[key] = glfwGetMouseButton(m_Window, key);
    return m_MouseState[key] && !m_OldMouseState[key];
}

bool Input::KeyPress(u16 key) {
    m_KeyboardState[key] = glfwGetKey(m_Window, key);
    return m_KeyboardState[key] != 0;
}

bool Input::KeyUp(u16 key) {
    m_KeyboardState[key] = glfwGetKey(m_Window, key);
    return !m_KeyboardState[key] && m_OldKeyboardState[key];
}

bool Input::KeyDown(u16 key) {
    m_KeyboardState[key] = glfwGetKey(m_Window, key);
    return m_KeyboardState[key] && !m_OldKeyboardState[key];
}

void Input::SetWindow(GLFWwindow *w) {
    m_Window = w;
}
