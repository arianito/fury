//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_MAT3_H
#define FURY_PRIMITIVES_MAT3_H

#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Uniform.h"
#include "Fury/Primitives/Vec3.h"
#include "Fury/Math.h"


struct Mat3 : Uniform<3, 3> {
    static const Mat3 Identify;

    Mat3()
            : Uniform<3, 3>
                      {1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 1.0f} {}

    Mat3(F32 scale = 1.0f)
            : Uniform<3, 3>
                      {scale, 0.0f, 0.0f,
                       0.0f, scale, 0.0f,
                       0.0f, 0.0f, scale} {}

    Mat3(F32 m00, F32 m01, F32 m02,
         F32 m10, F32 m11, F32 m12,
         F32 m20, F32 m21, F32 m22)
            : Uniform<3, 3>
                      {m00, m01, m02,
                       m10, m11, m12,
                       m20, m21, m22} {}

    Mat3(const Uniform &a) : Uniform(a) {}

    Mat3(const Uniform &&a) noexcept : Uniform{a} {}


    friend Mat3 operator*(const Mat3 &a, const Mat3 &b) {
        return {
                b[0] * a[0] + b[3] * a[1] + b[6] * a[2],
                b[1] * a[0] + b[4] * a[1] + b[7] * a[2],
                b[2] * a[0] + b[5] * a[1] + b[8] * a[2],

                b[0] * a[3] + b[3] * a[4] + b[6] * a[5],
                b[1] * a[3] + b[4] * a[4] + b[7] * a[5],
                b[2] * a[3] + b[5] * a[4] + b[8] * a[5],

                b[0] * a[6] + b[3] * a[7] + b[6] * a[8],
                b[1] * a[6] + b[4] * a[7] + b[7] * a[8],
                b[2] * a[6] + b[5] * a[7] + b[8] * a[8],

        };
    }

    friend Vec3 operator*(const Mat3 &a, const Vec3 &b) {
        return {
                a[0] * b[0] + a[1] * b[1] + a[2] * b[2],
                a[3] * b[0] + a[4] * b[1] + a[5] * b[2],
                a[6] * b[0] + a[7] * b[1] + a[8] * b[2]
        };
    }

    friend Mat3 operator*(const Mat3 &a, const F32 &b) {
        return {
                a[0] * b, a[1] * b, a[2] * b,
                a[3] * b, a[4] * b, a[5] * b,
                a[6] * b, a[7] * b, a[8] * b
        };
    }

    friend Mat3 operator*(const F32 &a, const Mat3 &b) {
        return {
                b[0] * a, b[1] * a, b[2] * a,
                b[3] * a, b[4] * a, b[5] * a,
                b[6] * a, b[7] * a, b[8] * a
        };
    }

    friend Mat3 operator+(const Mat3 &a, const Mat3 &b) {
        return {
                a[0] + b[0],
                a[1] + b[1],
                a[2] + b[2],
                a[3] + b[3],
                a[4] + b[4],
                a[5] + b[5],
                a[6] + b[6],
                a[7] + b[7],
                a[8] + b[8]
        };
    }

    friend Mat3 operator-(const Mat3 &a, const Mat3 &b) {
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
        };
    }

    friend Mat3 operator/(const Mat3 &a, const Mat3 &b) {
        return a * b.Invert();
    }

    friend Mat3 operator/(const Mat3 &a, const F32 &b) {
        return {
                a[0] / b, a[1] / b, a[2] / b,
                a[3] / b, a[4] / b, a[5] / b,
                a[6] / b, a[7] / b, a[8] / b,
        };
    }


    F32 Determinant() const {
        return data[0] * (data[4] * data[8] - data[5] * data[7]) -
               data[1] * (data[3] * data[8] - data[5] * data[6]) +
               data[2] * (data[3] * data[7] - data[4] * data[6]);
    }


    Mat3 Invert() const {
        auto det = this->Determinant();
        if (det == 0) return Mat3::Identify;
        return {
                (data[4] * data[8] - data[7] * data[5]) / det,
                (data[2] * data[7] - data[1] * data[8]) / det,
                (data[1] * data[5] - data[2] * data[4]) / det,
                (data[5] * data[6] - data[3] * data[8]) / det,
                (data[0] * data[8] - data[2] * data[6]) / det,
                (data[3] * data[2] - data[0] * data[5]) / det,
                (data[3] * data[7] - data[6] * data[4]) / det,
                (data[6] * data[1] - data[0] * data[7]) / det,
                (data[0] * data[4] - data[3] * data[1]) / det,
        };
    }


    static const Mat3 CreateTranslation(const Uniform<2, 1> &a) {
        return {
                1.0f, 0.0f, a[0],
                0.0f, 1.0f, a[1],
                0.0f, 0.0f, 1.0f
        };
    }

    static const Mat3 CreateRotation(const F32 &angle) {
        return {
                Math::Cos(angle), -Math::Sin(angle), 0.0f,
                Math::Sin(angle), Math::Cos(angle), 0.0f,
                0.0f, 0.0f, 1.0f
        };
    }


    static const Mat3 CreateScale(const Uniform<2, 1> &a) {
        return {
                a[0], 0.0f, 0.0f,
                0.0f, a[1], 0.0f,
                0.0f, 0.0f, 1.0f
        };
    }



};

const Mat3 Mat3::Identify{1.0f};

#endif //GAME_MAT4_H
