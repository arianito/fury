//
// Created by aryan on 3/20/20.
//

#include "fury/window.h"


i32 Window::run() {

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
    time = new Time();
    display = new Display();
    input = new Input(window, time);
    if (window == nullptr) {
        log_fatal("game: Failed to load OpenGL");
        throw;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 200, 200);

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        log_fatal("game: Failed to initialize OpenGL context");
        throw;
    }


    this->create();

    int frames{0};
    f32 now{0}, rate{0};
    f32 accumulator{0};
    f32 vel{0};

    glfwSetTime(0);
    do {
        input->update();
        GLint w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        display->width = (f32) w;
        display->height = (f32) h;
        rate = static_cast<f32>(glfwGetTime() - now);
        now = static_cast<f32>(glfwGetTime());
        time->deltaTime = rate;
        time->smoothDeltaTime = Math::smoothDamp(time->smoothDeltaTime, rate, vel, 0.5f, 100, time->fixedDeltaTime);
        time->elapsedTime = now;
        if (rate > 0.25f)
            rate = 0.25f;
        accumulator += rate;
        while (accumulator >= time->fixedDeltaTime) {
            this->fixedUpdate();
            accumulator -= time->fixedDeltaTime;
        }
        this->update();

        glClearColor(0.04, 0.2, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->render();


        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    this->dispose();
    glfwTerminate();
    return 0;
}
