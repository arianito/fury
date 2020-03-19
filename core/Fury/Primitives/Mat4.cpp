#include "Fury/Primitives/Mat4.h"
#include "Fury/Math.h"


const Mat4 Mat4::Identify{1.0f};

F32 Mat4::Determinant() const {
    return Mat4::Determinant(*this);
}

Mat4 Mat4::Invert() const {
    return Mat4::Invert(*this);
}

F32 Mat4::Determinant(const Mat4 &a) {
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

Mat4 Mat4::Invert(const Mat4 &a) {
    auto det = Mat4::Determinant(a);
    if (det == 0) return Mat4::Identify;
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

Mat4 Mat4::CreateTranslation(const Uniform<3, 1> &a) {
    return {
            1.0f, 0.0f, 0.0f, a[0],
            0.0f, 1.0f, 0.0f, a[1],
            0.0f, 0.0f, 1.0f, a[2],
            0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 Mat4::FromQuaternion(const Uniform<4, 1> &a) {
    F32 fTx = a[0] + a[0];
    F32 fTy = a[1] + a[1];
    F32 fTz = a[2] + a[2];
    F32 fTwx = fTx * a[3];
    F32 fTwy = fTy * a[3];
    F32 fTwz = fTz * a[3];
    F32 fTxx = fTx * a[0];
    F32 fTxy = fTy * a[0];
    F32 fTxz = fTz * a[0];
    F32 fTyy = fTy * a[1];
    F32 fTyz = fTz * a[1];
    F32 fTzz = fTz * a[2];

    return {
            1.0f - (fTyy + fTzz), fTxy - fTwz, fTxz + fTwy, 0.0f,
            fTxy + fTwz, 1.0f - (fTxx + fTzz), fTyz - fTwx, 0.0f,
            fTxz - fTwy, fTyz + fTwx, 1.0f - (fTxx + fTyy), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 Mat4::CreateScale(const Uniform<3, 1> &a) {
    return {
            a[0], 0.0f, 0.0f, 0.0f,
            0.0f, a[1], 0.0f, 0.0f,
            0.0f, 0.0f, a[2], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 Mat4::Perspective(const F32 &fov, const F32 &aspect, const F32 &near, const F32 &far) {
    F32 yScale = 1.0f / Math::Tan(fov / 2.0f);
    F32 xScale = yScale / aspect;
    F32 nmf = near - far;
    return {
            xScale, 0, 0, 0,
            0, yScale, 0, 0,
            0, 0, (far + near) / nmf, 2 * far * near / nmf,
            0, 0, -1, 0
    };
}

Mat4 Mat4::Orthographic(const F32 &top, const F32 &right, const F32 &bottom, const F32 &left, const F32 &near, const F32 &far, const F32 &zoom) {
    return {
            2.0f / (right - left), 0, 0, (right + left) / (left - right),
            0, 2.0f / (top - bottom), 0, (top + bottom) / (bottom - top),
            0, 0, 2.0f / (near - far), (far + near) / (near - far),
            0, 0, 0, zoom,
    };
}

Mat4 Mat4::LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
    Vec3 zaxis = (eye - center).Normal();
    Vec3 xaxis = Vec3::Cross(up, zaxis).Normal();
    Vec3 yaxis = Vec3::Cross(zaxis, xaxis);
    return {
            xaxis[0], xaxis[1], xaxis[2], -xaxis.Dot(eye),
            yaxis[0], yaxis[1], yaxis[2], -yaxis.Dot(eye),
            zaxis[0], zaxis[1], zaxis[2], -zaxis.Dot(eye),
            0, 0, 0, 1
    };
}




