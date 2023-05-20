#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "trace.h"
#include "game_time.h"
#include "system.h"
#include "display.h"
#include "debug_draw.h"
#include "game_input.h"
#include "mathf.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

struct Window {

    virtual void Destroy() {};

    virtual void Init() {};

    virtual void OnDrawGUI() {};

    virtual void Create() = 0;

    virtual void Update() = 0;

    virtual void FixedUpdate() {};

    virtual void Render() = 0;

    i32 run();

public:
    GLFWwindow *window = nullptr;
};