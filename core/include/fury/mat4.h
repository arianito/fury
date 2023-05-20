#pragma once

#include "system.h"
#include "uniform.h"
#include "vec3.h"


struct Mat4 : Uniform<4, 4> {
    static const Mat4 identify;

    Mat4()
            : Uniform<4, 4>
                      {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f} {}

    explicit Mat4(f32 scale)
            : Uniform<4, 4>
                      {scale, 0.0f, 0.0f, 0.0f,
                       0.0f, scale, 0.0f, 0.0f,
                       0.0f, 0.0f, scale, 0.0f,
                       0.0f, 0.0f, 0.0f, scale} {}

    Mat4(f32 m00, f32 m01, f32 m02, f32 m03,
         f32 m10, f32 m11, f32 m12, f32 m13,
         f32 m20, f32 m21, f32 m22, f32 m23,
         f32 m30, f32 m31, f32 m32, f32 m33)
            : Uniform<4, 4>
                      {m00, m01, m02, m03,
                       m10, m11, m12, m13,
                       m20, m21, m22, m23,
                       m30, m31, m32, m33} {}


    explicit Mat4(const Uniform &a) : Uniform(a) {}

    explicit Mat4(const Uniform &&a) noexcept : Uniform{a} {}

    f32 determinant() const;

    Mat4 invert() const;

    static Mat4 invert(const Mat4 &a);

    static f32 determinant(const Mat4 &a);

    static Mat4 createTranslation(const Uniform<3, 1> &a);

    static Mat4 fromQuaternion(const Uniform<4, 1> &a);

    static Mat4 createScale(const Uniform<3, 1> &a);

    static Mat4 perspective(const f32 &fov, const f32 &aspect, const f32 &near, const f32 &far);

    static Mat4
    orthographic(const f32 &top, const f32 &right, const f32 &bottom, const f32 &left, const f32 &near, const f32 &far,
                 const f32 &zoom = 1.0f);

    static Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);


    friend Mat4 operator*(const Mat4 &a, const Mat4 &b);

    friend Uniform<4, 1> operator*(const Mat4 &a, const Uniform<4, 1> &b);

    friend Uniform<4, 1> operator*(const Uniform<4, 1> &a, const Mat4 &b);

    friend Mat4 operator*(const Mat4 &a, const f32 &b);

    friend Mat4 operator*(const f32 &a, const Mat4 &b);

    friend Mat4 operator+(const Mat4 &a, const Mat4 &b);


    friend Mat4 operator-(const Mat4 &a, const Mat4 &b);

    friend Mat4 operator/(const Mat4 &a, const Mat4 &b);

    friend Mat4 operator/(const Mat4 &a, const f32 &b);

};
