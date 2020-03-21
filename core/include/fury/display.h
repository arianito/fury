
#ifndef FURY_GRAPHICS_DISPLAY_H
#define FURY_GRAPHICS_DISPLAY_H

struct Display {
    f32 width;
    f32 height;

    inline f32 ratio() {
        return width / height;
    }
};

#endif