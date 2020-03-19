
#ifndef FURY_GRAPHICS_WINDOW_H
#define FURY_GRAPHICS_WINDOW_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Fury/Trace.h"
#include "Fury/Graphics/Time.h"
#include "Fury/Primitives/System.h"
#include "Fury/Graphics/Display.h"

struct Window {

    virtual void Dispose() {};

    virtual void Create() {};

    virtual void Update() {};

    virtual void FixedUpdate() {};

    virtual void Draw() {};

    I32 Run() {

        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            return -1;
        }


        glfwWindowHint(GLFW_SAMPLES, 2);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(800, 600, "Aryan", NULL, NULL);

        if (window == nullptr) {
            Error("game: Failed to load OpenGL");
            throw;
        }
        glfwMakeContextCurrent(window);
        glfwSetWindowPos(window, 200, 200);

        glfwSwapInterval(1);
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            Error("game: Failed to initialize OpenGL context");
            throw;
        }



        this->Create();

        int frames{0};
        F32 now{0}, rate{0};
        F32 accumulator{0};
        F32 vel{0};

        glfwSetTime(0);
        do {
            GLint w, h;
            glfwGetFramebufferSize(window, &w, &h);
            glViewport(0, 0, w, h);
            Display::Width = (F32) w;
            Display::Height = (F32) h;
            rate = static_cast<F32>(glfwGetTime() - now);
            now = static_cast<F32>(glfwGetTime());
            Time::DeltaTime = rate;
            Time::SmoothDeltaTime = Math::SmoothDamp(Time::SmoothDeltaTime, rate, vel, 0.5f, 100, Time::FixedDeltaTime);
            Time::ElapsedTime = now;
            if (rate > 0.25f)
                rate = 0.25f;
            accumulator += rate;
            while (accumulator >= Time::FixedDeltaTime) {
                this->FixedUpdate();
                accumulator -= Time::FixedDeltaTime;
            }
            this->Update();
            this->Draw();


            glfwSwapBuffers(window);
            glfwPollEvents();
        } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

        this->Dispose();
        glfwTerminate();
        return 0;
    }

public:
    GLFWwindow *window = NULL;
};

#endif