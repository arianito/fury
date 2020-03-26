#pragma once

#include "system.h"

struct Time {
    static f32 ElapsedTime;
    static f32 DeltaTime;
    static f32 SmoothDeltaTime;
    static constexpr f32 FixedDeltaTime{1.0f / 60.0f};
};
