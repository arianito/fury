//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_QUATERNION_H
#define FURY_PRIMITIVES_QUATERNION_H

#include "system.h"
#include "uniform.h"
#include "vec3.h"
#include "vec4.h"


struct Quaternion : Uniform<4, 1> {
    static const Quaternion identify;

    Quaternion(f32 scale = 0.0f) : Uniform{scale, scale, scale, scale} {}

    Quaternion(f32 x, f32 y, f32 z = 0.0f, f32 w = 0.0f) : Uniform{x, y, z, w} {}

    Quaternion(const Uniform &a) : Uniform(a) {}

    Quaternion(const Uniform &&a) noexcept : Uniform{a} {}

    Vec4 toAngleAxis() const;


    static const Quaternion fromAngleAxis(const f32 &angle, const Vec3 &axis);

    static const Quaternion fromEulerAngles(const f32 &pitch, const f32 &roll, const f32 &yaw);

    static const Quaternion fromMatrix(const Uniform<4, 4> &m);


    friend Quaternion operator*(const Quaternion &a, const Quaternion &b) {
        return Quaternion{a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1],
                          a[3] * b[1] + a[1] * b[3] + a[2] * b[0] - a[0] * b[2],
                          a[3] * b[2] + a[2] * b[3] + a[0] * b[1] - a[1] * b[0],
                          a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2]};
    };

    friend Vec3 operator*(const Quaternion &a, const Vec3 &b) {
        f32 num = a[0] * 2.0f;
        f32 num2 = a[1] * 2.0f;
        f32 num3 = a[2] * 2.0f;
        f32 num4 = a[0] * num;
        f32 num5 = a[1] * num2;
        f32 num6 = a[2] * num3;
        f32 num7 = a[0] * num2;
        f32 num8 = a[0] * num3;
        f32 num9 = a[1] * num3;
        f32 num10 = a[3] * num;
        f32 num11 = a[3] * num2;
        f32 num12 = a[3] * num3;
        return Vec3{
                (1.0f - (num5 + num6)) * b[0] + (num7 - num12) * b[1] + (num8 + num11) * b[2],
                (num7 + num12) * b[0] + (1.0f - (num4 + num6)) * b[1] + (num9 - num10) * b[2],
                (num8 - num11) * b[0] + (num9 + num10) * b[1] + (1.0f - (num4 + num5)) * b[2]
        };
    };


};


#endif //GAME_QUAT_H
