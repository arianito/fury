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
    this->init();


    window = glfwCreateWindow(800, 600, "Aryan", nullptr, nullptr);
    time = Time::instance();
    display = Display::instance();
    input = Input::instance(window, time);
    for (auto &entity : entities) {
        entity->input = input;
        entity->display = display;
        entity->time = time;
        entity->window = window;
    }

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
    for (auto &entity : entities) {
        if (entity->isActive()){
            entity->create();
        }
    }
    this->create();
    for (auto &entity : entities) {
        if (entity->isActive()){
            entity->awake();
        }
    }

    f32 now{0}, rate{0};
    f32 accumulator{0};
    f32 vel{0};

    glfwSetTime(0);
    do {
        for (auto &entity : entities) {
            entity->validate();
            if(entity->isActive())
                entity->lateUpdate();
        }
        input->update();
        GLint w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
#if __APPLE__
        display->width = (f32) w/2;
        display->height = (f32) h/2;
#else
        display->width = (f32) w;
        display->height = (f32) h;
#endif
        rate = static_cast<f32>(glfwGetTime() - now);
        now = static_cast<f32>(glfwGetTime());
        time->deltaTime = rate;
        time->smoothDeltaTime = Math::smoothDamp(time->smoothDeltaTime, rate, vel, 0.5f, 100, time->fixedDeltaTime);
        time->elapsedTime = now;
        if (rate > 0.25f)
            rate = 0.25f;
        accumulator += rate;
        while (accumulator >= time->fixedDeltaTime) {
            for (auto &entity : entities) {
                if (entity->isActive())
                    entity->fixedUpdate();
            }
            this->fixedUpdate();
            accumulator -= time->fixedDeltaTime;
        }
        for (auto &entity : entities) {
            if (entity->isActive())
                entity->update();
        }
        this->update();

        glClearColor(0.1, 0.2, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (auto &entity : entities) {
            if (entity->isActive())
                entity->render();
        }
        this->render();


        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    for (auto &entity : entities) {
        entity->dispose();
    }
    this->dispose();
    glfwTerminate();
    return 0;
}

void Window::add(Entity *entity) {
    entities.emplace_back(entity);
}


void Window::remove(Entity *entity) {
    entities.remove(entity);
}
