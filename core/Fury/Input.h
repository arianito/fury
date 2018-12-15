#ifndef FURY_INPUT_H
#define FURY_INPUT_H

#include <GLFW/glfw3.h>
#include <Fury/Graphics/Time.h>

static int keyboardState[350];
static int lastKeyboardState[350];


static const int axLength = 2;

static float axises[axLength];
static float axisesVelocity[axLength];

static GLFWwindow *gwin;


enum Axis {
    Vertical = 0,
    Horizontal = 1,
};

enum Keys {
    Unknown = -1,
    SPace = 32,
    Apostrophe= 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    Num0 = 48,
    Num1 = 49,
    Num2 = 50,
    Num3 = 51,
    Num4 = 52,
    Num5 = 53,
    Num6 = 54,
    Num7 = 55,
    Num8 = 56,
    Num9 = 57,
    Semicolon = 59,
    Equal = 61,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,
    World1 = 161,
    World2 = 162,
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    Keypad0 = 320,
    Keypad1 = 321,
    Keypad2 = 322,
    Keypad3 = 323,
    Keypad4 = 324,
    Keypad5 = 325,
    Keypad6 = 326,
    Keypad7 = 327,
    Keypad8 = 328,
    Keypad9 = 329,
    KeypadDecimal = 330,
    KeypadDivide = 331,
    KeypadMultiply = 332,
    KeypadSubtract = 333,
    KeypadAdd = 334,
    KeypadEnter = 335,
    KeypadEqual = 336,
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,
};


struct Input {

    static void Init(GLFWwindow *window) {
        gwin = window;
    }


    static bool KeyDown(Keys key) {
        if (key < 0)
            return false;
        lastKeyboardState[key] = keyboardState[key];
        keyboardState[key] = glfwGetKey(gwin, key);
        return keyboardState[key] && !lastKeyboardState[key];
    }

    static bool KeyUp(Keys key) {
        if (key < 0)
            return false;
        lastKeyboardState[key] = keyboardState[key];
        keyboardState[key] = glfwGetKey(gwin, key);
        return !keyboardState[key] && lastKeyboardState[key];
    }


    static bool KeyPress(Keys key) {
        if (key < 0)
            return false;
        keyboardState[key] = glfwGetKey(gwin, key);
        return keyboardState[key] != 0;
    }


    static float GetAxis(Axis a) {
        return axises[a];
    }


    static void Update() {
        moveAxis(Axis::Horizontal, KeyPress(Keys::Left) || KeyPress(Keys::A), KeyPress(Keys::Right) || KeyPress(Keys::D));
        moveAxis(Axis::Vertical, KeyPress(Keys::Down) || KeyPress(Keys::S), KeyPress(Keys::Up) || KeyPress(Keys::W));
    }

private:

    static void moveAxis(Axis ax, bool low, bool high) {
        float delta = Time::DeltaTime;
        float to = 0 + (low ? -1 : 0) + (high ? 1 : 0);
        if (fabsf(axises[ax] - to) > Math::KEPSILON) {
            axises[ax] = Math::SmoothDamp(axises[ax], to, axisesVelocity[ax], 0.2f, 40, delta);
        }

    }
};

#endif