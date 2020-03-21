//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_MAT3_H
#define FURY_PRIMITIVES_MAT3_H

#include "system.h"
#include "uniform.h"
#include "vec3.h"
#include "mathf.h"


struct Mat3 : Uniform<3, 3> {
    static const Mat3 identify;

    Mat3()
            : Uniform<3, 3>
                      {1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 1.0f} {}

    explicit Mat3(f32 scale = 1.0f)
            : Uniform<3, 3>
                      {scale, 0.0f, 0.0f,
                       0.0f, scale, 0.0f,
                       0.0f, 0.0f, scale} {}

    Mat3(f32 m00, f32 m01, f32 m02,
         f32 m10, f32 m11, f32 m12,
         f32 m20, f32 m21, f32 m22)
            : Uniform<3, 3>
                      {m00, m01, m02,
                       m10, m11, m12,
                       m20, m21, m22} {}

    explicit Mat3(const Uniform &a) : Uniform(a) {}

    explicit Mat3(const Uniform &&a) noexcept : Uniform{a} {}


    friend Mat3 operator*(const Mat3 &a, const Mat3 &b);

    friend Vec3 operator*(const Mat3 &a, const Vec3 &b);

    friend Mat3 operator*(const Mat3 &a, const f32 &b);

    friend Mat3 operator*(const f32 &a, const Mat3 &b);

    friend Mat3 operator+(const Mat3 &a, const Mat3 &b);

    friend Mat3 operator-(const Mat3 &a, const Mat3 &b);

    friend Mat3 operator/(const Mat3 &a, const Mat3 &b);

    friend Mat3 operator/(const Mat3 &a, const f32 &b);


    [[nodiscard]] f32 determinant() const;


    [[nodiscard]] Mat3 invert() const;


    static Mat3 createTranslation(const Uniform<2, 1> &a);

    static Mat3 createRotation(const f32 &angle);


    static Mat3 createScale(const Uniform<2, 1> &a);
};

#endif //GAME_MAT4_H
