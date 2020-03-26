#pragma once

#include "system.h"

struct Display {
    static f32 Width;
    static f32 Height;

    static inline f32 GetRatio() {
        return Width / Height;
    }
};