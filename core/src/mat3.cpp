//
// Created by aryan on 3/20/20.
//

#include "fury/mat3.h"


const Mat3 Mat3::identify{1.0f};

Mat3 operator*(const Mat3 &a, const Mat3 &b) {
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

Mat3 Mat3::createScale(const Uniform<2, 1> &a) {
    return {
            a[0], 0.0f, 0.0f,
            0.0f, a[1], 0.0f,
            0.0f, 0.0f, 1.0f
    };
}

Mat3 Mat3::createRotation(const f32 &angle) {
    return {
            Math::cos(angle), -Math::sin(angle), 0.0f,
            Math::sin(angle), Math::cos(angle), 0.0f,
            0.0f, 0.0f, 1.0f
    };
}

Mat3 Mat3::createTranslation(const Uniform<2, 1> &a) {
    return {
            1.0f, 0.0f, a[0],
            0.0f, 1.0f, a[1],
            0.0f, 0.0f, 1.0f
    };
}

Mat3 Mat3::invert() const {
    auto det = this->determinant();
    if (det == 0) return Mat3::identify;
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

f32 Mat3::determinant() const {
    return data[0] * (data[4] * data[8] - data[5] * data[7]) -
           data[1] * (data[3] * data[8] - data[5] * data[6]) +
           data[2] * (data[3] * data[7] - data[4] * data[6]);
}

Mat3 operator/(const Mat3 &a, const f32 &b) {
    return {
            a[0] / b, a[1] / b, a[2] / b,
            a[3] / b, a[4] / b, a[5] / b,
            a[6] / b, a[7] / b, a[8] / b,
    };
}

Mat3 operator/(const Mat3 &a, const Mat3 &b) {
    return a * b.invert();
}

Mat3 operator-(const Mat3 &a, const Mat3 &b) {
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

Mat3 operator+(const Mat3 &a, const Mat3 &b) {
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

Mat3 operator*(const f32 &a, const Mat3 &b) {
    return {
            b[0] * a, b[1] * a, b[2] * a,
            b[3] * a, b[4] * a, b[5] * a,
            b[6] * a, b[7] * a, b[8] * a
    };
}

Mat3 operator*(const Mat3 &a, const f32 &b) {
    return {
            a[0] * b, a[1] * b, a[2] * b,
            a[3] * b, a[4] * b, a[5] * b,
            a[6] * b, a[7] * b, a[8] * b
    };
}

Vec3 operator*(const Mat3 &a, const Vec3 &b) {
    return {
            a[0] * b[0] + a[1] * b[1] + a[2] * b[2],
            a[3] * b[0] + a[4] * b[1] + a[5] * b[2],
            a[6] * b[0] + a[7] * b[1] + a[8] * b[2]
    };
}
