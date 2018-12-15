//
// Created by Aryan on 10/29/18.
//

#ifndef FURY_GRAPHICS_TIME_H
#define FURY_GRAPHICS_TIME_H


struct Time {
    static F32 ElapsedTime;
    static F32 DeltaTime;
    static F32 SmoothDeltaTime;
    static constexpr F32 FixedDeltaTime{1.0f / 60.0f};
};

F32 Time::ElapsedTime{0};
F32 Time::DeltaTime{0};
F32 Time::SmoothDeltaTime{0};

#endif //ORBITAL_GL_TIME_H
