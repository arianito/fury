
#ifndef FURY_GRAPHICS_WINDOW_H
#define FURY_GRAPHICS_WINDOW_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "trace.h"
#include "timef.h"
#include "system.h"
#include "display.h"
#include "game-input.h"
#include "mathf.h"
#include "entity.h"

struct Window {

    virtual void dispose() {};

    virtual void init() {};

    virtual void gui() {};

    virtual void create() {};

    virtual void update() {};

    virtual void fixedUpdate() {};

    virtual void render() {};

    i32 run();

    void add(Entity *entity);

    void remove(Entity *entity);

public:
    GLFWwindow *window = nullptr;
    Input *input = nullptr;
    Time *time = nullptr;
    Display *display = nullptr;
    List<Entity *> entities;
};

#endif