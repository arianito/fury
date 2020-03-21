//
// Created by aryan on 3/20/20.
//

#include "fury/display.h"

Display *Display::m_instance = nullptr;

Display *Display::instance() {
    if (!m_instance)
        m_instance = new Display();
    return m_instance;
}
