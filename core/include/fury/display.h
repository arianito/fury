
#ifndef FURY_GRAPHICS_DISPLAY_H
#define FURY_GRAPHICS_DISPLAY_H

#include "system.h"

struct Display {
    static Display *instance();

    f32 width;
    f32 height;

    inline f32 ratio() {
        return width / height;
    }

private:
    static Display *m_instance;
};

#endif