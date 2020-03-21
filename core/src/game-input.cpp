//
// Created by aryan on 3/21/20.
//

#include "fury/game-input.h"

void Input::moveAxis(u16 ax, bool low, bool high) {
    auto delta = time->deltaTime;
    float to = 0.0f + (low ? -1.0f : 0.0f) + (high ? 1.0f : 0.0f);
    if (fabsf(axises[ax] - to) > Math::KEPSILON) {
        axises[ax] = Math::smoothDamp(axises[ax], to, axisesVelocity[ax], 0.2f, 40, delta);
    }

}

void Input::update() {
    moveAxis(AXIS_HORIZONTAL, keyPress(KEY_LEFT) || keyPress(KEY_A), keyPress(KEY_RIGHT) || keyPress(KEY_D));
    moveAxis(AXIS_VERTICAL, keyPress(KEY_DOWN) || keyPress(KEY_S), keyPress(KEY_UP) || keyPress(KEY_W));
    glfwGetCursorPos(gwin, &xpos, &ypos);
}

float Input::getAxis(u8 a) {
    return axises[a];
}

Vec2 Input::position() const {
    return {(f32) xpos, (f32) ypos};
}

f32 Input::y() const {
    return ypos;
}

f32 Input::x() const {
    return xpos;
}

bool Input::mousePress(u16 key) {
    if (key < 0)
        return false;
    return glfwGetMouseButton(gwin, key) != 0;
}

bool Input::mouseUp(u16 key) {
    if (key < 0)
        return false;
    lastMouseState[key] = mouseState[key];
    mouseState[key] = glfwGetMouseButton(gwin, key);
    return !mouseState[key] && lastMouseState[key];
}

bool Input::mouseDown(u16 key) {
    if (key < 0)
        return false;
    lastMouseState[key] = mouseState[key];
    mouseState[key] = glfwGetMouseButton(gwin, key);
    return mouseState[key] && !lastMouseState[key];
}

bool Input::keyPress(u16 key) {
    if (key < 0)
        return false;
    return glfwGetKey(gwin, key) != 0;
}

bool Input::keyUp(u16 key) {
    if (key < 0)
        return false;
    lastKeyboardState[key] = keyboardState[key];
    keyboardState[key] = glfwGetKey(gwin, key);
    return !keyboardState[key] && lastKeyboardState[key];
}

bool Input::keyDown(u16 key) {
    if (key < 0)
        return false;
    lastKeyboardState[key] = keyboardState[key];
    keyboardState[key] = glfwGetKey(gwin, key);
    return keyboardState[key] && !lastKeyboardState[key];
}

Input *Input::m_instance = nullptr;

Input::Input(GLFWwindow *window, Time *t) {
    gwin = window;
    time = t;
}

Input *Input::instance(GLFWwindow *window, Time *t) {
    if (!m_instance)
        m_instance = new Input(window, t);
    return m_instance;
}

Input *Input::instance() {
    return m_instance;
}
