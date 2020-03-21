
#ifndef FURY_GRAPHICS_WINDOW_H
#define FURY_GRAPHICS_WINDOW_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "trace.h"
#include "timef.h"
#include "system.h"
#include "display.h"
#include "input.h"
#include "mathf.h"

struct Window {

    virtual void dispose() {};

    virtual void create() {};

    virtual void update() {};

    virtual void fixedUpdate() {};

    virtual void render() {};

    i32 run();

public:
    GLFWwindow *window = nullptr;
    Input *input;
    Time *time;
    Display *display;
};

#endif