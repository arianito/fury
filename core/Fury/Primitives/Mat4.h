//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_MAT4_H
#define FURY_PRIMITIVES_MAT4_H

#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Uniform.h"
#include "Fury/Primitives/Vec3.h"


struct Mat4 : Uniform<4, 4> {
    static const Mat4 Identify;

    Mat4()
            : Uniform<4, 4>
                      {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f} {}

    Mat4(F32 scale = 1.0f)
            : Uniform<4, 4>
                      {scale, 0.0f, 0.0f, 0.0f,
                       0.0f, scale, 0.0f, 0.0f,
                       0.0f, 0.0f, scale, 0.0f,
                       0.0f, 0.0f, 0.0f, scale} {}

    Mat4(F32 m00, F32 m01, F32 m02, F32 m03,
         F32 m10, F32 m11, F32 m12, F32 m13,
         F32 m20, F32 m21, F32 m22, F32 m23,
         F32 m30, F32 m31, F32 m32, F32 m33)
            : Uniform<4, 4>
                      {m00, m01, m02, m03,
                       m10, m11, m12, m13,
                       m20, m21, m22, m23,
                       m30, m31, m32, m33} {}


    Mat4(const Uniform &a) : Uniform(a) {}

    Mat4(const Uniform &&a) noexcept : Uniform{a} {}

    F32 Determinant() const;

    Mat4 Invert() const;

    static Mat4 Invert(const Mat4 &a);

    static F32 Determinant(const Mat4 &a);

    static Mat4 CreateTranslation(const Uniform<3, 1> &a);

    static Mat4 FromQuaternion(const Uniform<4, 1> &a);

    static Mat4 CreateScale(const Uniform<3, 1> &a);

    static Mat4 Perspective(const F32 &fov, const F32 &aspect, const F32 &near, const F32 &far);

    static Mat4 Orthographic(const F32 &top, const F32 &right, const F32 &bottom, const F32 &left, const F32 &near, const F32 &far, const F32 &zoom = 1.0f);

    static Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);


    friend Mat4 operator*(const Mat4 &a, const Mat4 &b) {
        return {
                b[0] * a[0] + b[4] * a[1] + b[8] * a[2] + b[12] * a[3],
                b[1] * a[0] + b[5] * a[1] + b[9] * a[2] + b[13] * a[3],
                b[2] * a[0] + b[6] * a[1] + b[10] * a[2] + b[14] * a[3],
                b[3] * a[0] + b[7] * a[1] + b[11] * a[2] + b[15] * a[3],

                b[0] * a[4] + b[4] * a[5] + b[8] * a[6] + b[12] * a[7],
                b[1] * a[4] + b[5] * a[5] + b[9] * a[6] + b[13] * a[7],
                b[2] * a[4] + b[6] * a[5] + b[10] * a[6] + b[14] * a[7],
                b[3] * a[4] + b[7] * a[5] + b[11] * a[6] + b[15] * a[7],

                b[0] * a[8] + b[4] * a[9] + b[8] * a[10] + b[12] * a[11],
                b[1] * a[8] + b[5] * a[9] + b[9] * a[10] + b[13] * a[11],
                b[2] * a[8] + b[6] * a[9] + b[10] * a[10] + b[14] * a[11],
                b[3] * a[8] + b[7] * a[9] + b[11] * a[10] + b[15] * a[11],

                b[0] * a[12] + b[4] * a[13] + b[8] * a[14] + b[12] * a[15],
                b[1] * a[12] + b[5] * a[13] + b[9] * a[14] + b[13] * a[15],
                b[2] * a[12] + b[6] * a[13] + b[10] * a[14] + b[14] * a[15],
                b[3] * a[12] + b[7] * a[13] + b[11] * a[14] + b[15] * a[15]

        };
    }

    friend Uniform<4, 1> operator*(const Mat4 &a, const Uniform<4, 1> &b) {
        return {
                a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3],
                a[4] * b[0] + a[5] * b[1] + a[6] * b[2] + a[7] * b[3],
                a[8] * b[0] + a[9] * b[1] + a[10] * b[2] + a[11] * b[3],
                a[12] * b[0] + a[13] * b[1] + a[14] * b[2] + a[15] * b[3],
        };
    }

    friend Mat4 operator*(const Mat4 &a, const F32 &b) {
        return {
                a[0] * b, a[1] * b, a[2] * b, a[3] * b,
                a[4] * b, a[5] * b, a[6] * b, a[7] * b,
                a[8] * b, a[9] * b, a[10] * b, a[11] * b,
                a[12] * b, a[13] * b, a[14] * b, a[15] * b,
        };
    }

    friend Mat4 operator*(const F32 &a, const Mat4 &b) {
        return {
                b[0] * a, b[1] * a, b[2] * a, b[3] * a, b[4] * a, b[5] * a, b[6] * a, b[7] * a, b[8] * a, b[9] * a,
                b[10] * a, b[11] * a, b[12] * a, b[13] * a, b[14] * a, b[15] * a,
        };
    }

    friend Mat4 operator+(const Mat4 &a, const Mat4 &b) {
        return {
                a[0] + b[0],
                a[1] + b[1],
                a[2] + b[2],
                a[3] + b[3],
                a[4] + b[4],
                a[5] + b[5],
                a[6] + b[6],
                a[7] + b[7],
                a[8] + b[8],
                a[9] + b[9],
                a[10] + b[10],
                a[11] + b[11],
                a[12] + b[12],
                a[13] + b[13],
                a[14] + b[14],
                a[15] + b[15],
        };
    }


    friend Mat4 operator-(const Mat4 &a, const Mat4 &b) {
        return {
                a[0] - b[0],
                a[1] - b[1],
                a[2] - b[2],
                a[3] - b[3],
                a[4] - b[4],
                a[5] - b[5],
                a[6] - b[6],
                a[7] - b[7],
                a[8] - b[8],
                a[9] - b[9],
                a[10] - b[10],
                a[11] - b[11],
                a[12] - b[12],
                a[13] - b[13],
                a[14] - b[14],
                a[15] - b[15],
        };
    }

    friend Mat4 operator/(const Mat4 &a, const Mat4 &b) {
        return a * b.Invert();
    }

    friend Mat4 operator/(const Mat4 &a, const F32 &b) {
        return {
                a[0] / b, a[1] / b, a[2] / b, a[3] / b, a[4] / b, a[5] / b, a[6] / b, a[7] / b, a[8] / b, a[9] / b,
                a[10] / b, a[11] / b, a[12] / b, a[13] / b, a[14] / b, a[15] / b,
        };
    }

};


#endif //GAME_MAT4_H
