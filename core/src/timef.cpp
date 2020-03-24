//
// Created by aryan on 3/20/20.
//

#include "fury/timef.h"

Time *Time::m_instance = nullptr;

Time *Time::instance() {
    if (!m_instance)
        m_instance = new Time();
    return m_instance;
}
