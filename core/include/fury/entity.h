//
// Created by aryan on 3/21/20.
//

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H


#include "game-input.h"
#include "timef.h"
#include "display.h"
#include "trace.h"

struct Entity {
    virtual void create() {};

    virtual void awake() {};

    virtual void sleep() {};

    virtual void gui() {};

    virtual void dispose() {};

    virtual void lateUpdate() {};

    virtual void update() {};

    virtual void fixedUpdate() {};

    virtual void render() {};

    inline void activate() {
        m_active_request = true;
    }

    inline void deactivate() {
        m_active_request = false;
    }

    inline bool isActive() {
        return m_active;
    }

    inline bool isDisposed() {
        return m_disposed;
    }

    inline void validate() {
        if (m_active != m_active_request) {
            if (m_active_request) {
                if (this->m_disposed) {
                    this->awake();
                    this->m_disposed = false;
                }
            } else {
                if (!this->m_disposed) {
                    this->sleep();
                    this->m_disposed = true;
                }
            }
            m_active = m_active_request;
        }
    }

    GLFWwindow *window = nullptr;
    Input *input = nullptr;
    Time *time = nullptr;
    Display *display = nullptr;
private:
    bool m_disposed = false;
    bool m_active = true;
    bool m_active_request = true;
};

#endif //GAME_ENTITY_H
