#include "fury/mat4.h"
#include "fury/mathf.h"


const Mat4 Mat4::identify{1.0f};

f32 Mat4::determinant() const {
    return Mat4::determinant(*this);
}

Mat4 Mat4::invert() const {
    return Mat4::invert(*this);
}

f32 Mat4::determinant(const Mat4 &a) {
    return a[0] * a[5] * a[10] * a[15] -
           a[0] * a[5] * a[11] * a[14] -
           a[0] * a[6] * a[9] * a[15] +
           a[0] * a[6] * a[11] * a[13] +
           a[0] * a[7] * a[9] * a[14] -
           a[0] * a[7] * a[10] * a[13] -
           a[1] * a[4] * a[10] * a[15] +
           a[1] * a[4] * a[11] * a[14] +
           a[1] * a[6] * a[8] * a[15] -
           a[1] * a[6] * a[11] * a[12] -
           a[1] * a[7] * a[8] * a[14] +
           a[1] * a[7] * a[10] * a[12] +
           a[2] * a[4] * a[9] * a[15] -
           a[2] * a[4] * a[11] * a[13] -
           a[2] * a[5] * a[8] * a[15] +
           a[2] * a[5] * a[11] * a[12] +
           a[2] * a[7] * a[8] * a[13] -
           a[2] * a[7] * a[9] * a[12] -
           a[3] * a[4] * a[9] * a[14] +
           a[3] * a[4] * a[10] * a[13] +
           a[3] * a[5] * a[8] * a[14] -
           a[3] * a[5] * a[10] * a[12] -
           a[3] * a[6] * a[8] * a[13] +
           a[3] * a[6] * a[9] * a[12];
}

Mat4 Mat4::invert(const Mat4 &a) {
    auto det = Mat4::determinant(a);
    if (det == 0) return Mat4::identify;
    return {
            (a[5] * a[10] * a[15] + a[6] * a[11] * a[13] +
             a[7] * a[9] * a[14] - a[5] * a[11] * a[14] -
             a[6] * a[9] * a[15] - a[7] * a[10] * a[13]) / det,

            (a[1] * a[11] * a[14] + a[2] * a[9] * a[15] +
             a[3] * a[10] * a[13] - a[1] * a[10] * a[15] -
             a[2] * a[11] * a[13] - a[3] * a[9] * a[14]) / det,

            (a[1] * a[6] * a[15] + a[2] * a[7] * a[13] +
             a[3] * a[5] * a[14] - a[1] * a[7] * a[14] -
             a[2] * a[5] * a[15] - a[3] * a[6] * a[13]) / det,

            (a[1] * a[7] * a[10] + a[2] * a[5] * a[11] +
             a[3] * a[6] * a[9] - a[1] * a[6] * a[11] -
             a[2] * a[7] * a[9] - a[3] * a[5] * a[10]) / det,

            (a[4] * a[11] * a[14] + a[6] * a[8] * a[15] +
             a[7] * a[10] * a[12] - a[4] * a[10] * a[15] -
             a[6] * a[11] * a[12] - a[7] * a[8] * a[14]) / det,

            (a[0] * a[10] * a[15] + a[2] * a[11] * a[12] +
             a[3] * a[8] * a[14] - a[0] * a[11] * a[14] -
             a[2] * a[8] * a[15] - a[3] * a[10] * a[12]) / det,

            (a[0] * a[7] * a[14] + a[2] * a[4] * a[15] +
             a[3] * a[6] * a[12] - a[0] * a[6] * a[15] -
             a[2] * a[7] * a[12] - a[3] * a[4] * a[14]) / det,

            (a[0] * a[6] * a[11] + a[2] * a[7] * a[8] +
             a[3] * a[4] * a[10] - a[0] * a[7] * a[10] -
             a[2] * a[4] * a[11] - a[3] * a[6] * a[8]) / det,

            (a[4] * a[9] * a[15] + a[5] * a[11] * a[12] +
             a[7] * a[8] * a[13] - a[4] * a[11] * a[13] -
             a[5] * a[8] * a[15] - a[7] * a[9] * a[12]) / det,

            (a[0] * a[11] * a[13] + a[1] * a[8] * a[15] +
             a[3] * a[9] * a[12] - a[0] * a[9] * a[15] -
             a[1] * a[11] * a[12] - a[3] * a[8] * a[13]) / det,

            (a[0] * a[5] * a[15] + a[1] * a[7] * a[12] +
             a[3] * a[4] * a[13] - a[0] * a[7] * a[13] -
             a[1] * a[4] * a[15] - a[3] * a[5] * a[12]) / det,

            (a[0] * a[7] * a[9] + a[1] * a[4] * a[11] +
             a[3] * a[5] * a[8] - a[0] * a[5] * a[11] -
             a[1] * a[7] * a[8] - a[3] * a[4] * a[9]) / det,

            (a[4] * a[10] * a[13] + a[5] * a[8] * a[14] +
             a[6] * a[9] * a[12] - a[4] * a[9] * a[14] -
             a[5] * a[10] * a[12] - a[6] * a[8] * a[13]) / det,

            (a[0] * a[9] * a[14] + a[1] * a[10] * a[12] +
             a[2] * a[8] * a[13] - a[0] * a[10] * a[13] -
             a[1] * a[8] * a[14] - a[2] * a[9] * a[12]) / det,

            (a[0] * a[6] * a[13] + a[1] * a[4] * a[14] +
             a[2] * a[5] * a[12] - a[0] * a[5] * a[14] -
             a[1] * a[6] * a[12] - a[2] * a[4] * a[13]) / det,

            (a[0] * a[5] * a[10] + a[1] * a[6] * a[8] +
             a[2] * a[4] * a[9] - a[0] * a[6] * a[9] -
             a[1] * a[4] * a[10] - a[2] * a[5] * a[8]) / det
    };
}

Mat4 Mat4::createTranslation(const Uniform<3, 1> &a) {
    return {
            1.0f, 0.0f, 0.0f, a[0],
            0.0f, 1.0f, 0.0f, a[1],
            0.0f, 0.0f, 1.0f, a[2],
            0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 Mat4::fromQuaternion(const Uniform<4, 1> &a) {
    f32 fTx = a[0] + a[0];
    f32 fTy = a[1] + a[1];
    f32 fTz = a[2] + a[2];
    f32 fTwx = fTx * a[3];
    f32 fTwy = fTy * a[3];
    f32 fTwz = fTz * a[3];
    f32 fTxx = fTx * a[0];
    f32 fTxy = fTy * a[0];
    f32 fTxz = fTz * a[0];
    f32 fTyy = fTy * a[1];
    f32 fTyz = fTz * a[1];
    f32 fTzz = fTz * a[2];

    return {
            1.0f - (fTyy + fTzz), fTxy - fTwz, fTxz + fTwy, 0.0f,
            fTxy + fTwz, 1.0f - (fTxx + fTzz), fTyz - fTwx, 0.0f,
            fTxz - fTwy, fTyz + fTwx, 1.0f - (fTxx + fTyy), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 Mat4::createScale(const Uniform<3, 1> &a) {
    return {
            a[0], 0.0f, 0.0f, 0.0f,
            0.0f, a[1], 0.0f, 0.0f,
            0.0f, 0.0f, a[2], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 Mat4::perspective(const f32 &fov, const f32 &aspect, const f32 &near, const f32 &far) {
    f32 yScale = 1.0f / Math::tan(fov / 2.0f);
    f32 xScale = yScale / aspect;
    f32 nmf = near - far;
    return {
            xScale, 0, 0, 0,
            0, yScale, 0, 0,
            0, 0, (far + near) / nmf, 2 * far * near / nmf,
            0, 0, -1, 0
    };
}

Mat4 Mat4::orthographic(const f32 &top, const f32 &right, const f32 &bottom, const f32 &left, const f32 &near, const f32 &far, const f32 &zoom) {
    return {
            2.0f / (right - left), 0, 0, (right + left) / (left - right),
            0, 2.0f / (top - bottom), 0, (top + bottom) / (bottom - top),
            0, 0, 2.0f / (near - far), (far + near) / (near - far),
            0, 0, 0, zoom,
    };
}

Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
    Vec3 zaxis = (eye - center).normal();
    Vec3 xaxis = Vec3::cross(up, zaxis).normal();
    Vec3 yaxis = Vec3::cross(zaxis, xaxis);
    return {
            xaxis[0], xaxis[1], xaxis[2], -xaxis.dot(eye),
            yaxis[0], yaxis[1], yaxis[2], -yaxis.dot(eye),
            zaxis[0], zaxis[1], zaxis[2], -zaxis.dot(eye),
            0, 0, 0, 1
    };
}

Mat4 operator*(const Mat4 &a, const Mat4 &b) {
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

Uniform<4, 1> operator*(const Mat4 &a, const Uniform<4, 1> &b) {
    return {
            a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3],
            a[4] * b[0] + a[5] * b[1] + a[6] * b[2] + a[7] * b[3],
            a[8] * b[0] + a[9] * b[1] + a[10] * b[2] + a[11] * b[3],
            a[12] * b[0] + a[13] * b[1] + a[14] * b[2] + a[15] * b[3],
    };
}

Uniform<4, 1> operator*(const Uniform<4, 1> &b, const Mat4 &a) {
    return {
            a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3],
            a[4] * b[0] + a[5] * b[1] + a[6] * b[2] + a[7] * b[3],
            a[8] * b[0] + a[9] * b[1] + a[10] * b[2] + a[11] * b[3],
            a[12] * b[0] + a[13] * b[1] + a[14] * b[2] + a[15] * b[3],
    };
}

Mat4 operator*(const Mat4 &a, const f32 &b) {
    return {
            a[0] * b, a[1] * b, a[2] * b, a[3] * b,
            a[4] * b, a[5] * b, a[6] * b, a[7] * b,
            a[8] * b, a[9] * b, a[10] * b, a[11] * b,
            a[12] * b, a[13] * b, a[14] * b, a[15] * b,
    };
}

Mat4 operator*(const f32 &a, const Mat4 &b) {
    return {
            b[0] * a, b[1] * a, b[2] * a, b[3] * a, b[4] * a, b[5] * a, b[6] * a, b[7] * a, b[8] * a, b[9] * a,
            b[10] * a, b[11] * a, b[12] * a, b[13] * a, b[14] * a, b[15] * a,
    };
}

Mat4 operator+(const Mat4 &a, const Mat4 &b) {
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

Mat4 operator-(const Mat4 &a, const Mat4 &b) {
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

Mat4 operator/(const Mat4 &a, const Mat4 &b) {
    return a * b.invert();
}

Mat4 operator/(const Mat4 &a, const f32 &b) {
    return {
            a[0] / b, a[1] / b, a[2] / b, a[3] / b, a[4] / b, a[5] / b, a[6] / b, a[7] / b, a[8] / b, a[9] / b,
            a[10] / b, a[11] / b, a[12] / b, a[13] / b, a[14] / b, a[15] / b,
    };
}




