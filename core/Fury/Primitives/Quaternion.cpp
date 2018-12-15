#include "Fury/Primitives/Quaternion.h"
#include "Fury/Math.h"


const Quaternion Quaternion::Identify{0.0f, 0.0f, 0.0f, 1.0f};


const Quaternion Quaternion::FromAngleAxis(const F32 &angle, const Vec3 &axis) {
    auto ha = 0.5 * angle;
    auto sn = Math::Sin(ha);

    return Quaternion{
            sn * axis[0],
            sn * axis[1],
            sn * axis[2],
            Math::Cos(ha)
    };
}

const Quaternion Quaternion::FromEulerAngles(const F32 &pitch, const F32 &roll, const F32 &yaw) {
    F32 cy = Math::Cos(yaw * 0.5f);
    F32 sy = Math::Sin(yaw * 0.5f);
    F32 cr = Math::Cos(roll * 0.5f);
    F32 sr = Math::Sin(roll * 0.5f);
    F32 cp = Math::Cos(pitch * 0.5f);
    F32 sp = Math::Sin(pitch * 0.5f);

    return Quaternion{
            cy * cr * cp + sy * sr * sp,
            cy * sr * cp - sy * cr * sp,
            cy * cr * sp + sy * sr * cp,
            sy * cr * cp - cy * sr * sp,
    };
}

const Quaternion Quaternion::FromMatrix(const Uniform<4, 4> &m) {
    auto q = Quaternion::Identify;
    F32 fTrace = m.data[0] + m.data[5] + m.data[10];
    F32 fRoot;
    if (fTrace > 0.0) {
        fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
        q[3] = 0.5f * fRoot;
        fRoot = 0.5f / fRoot;
        q[0] = (m.data[6] - m.data[9]) * fRoot;
        q[1] = (m.data[8] - m.data[2]) * fRoot;
        q[2] = (m.data[1] - m.data[4]) * fRoot;
    } else {
        // |w| <= 1/2
        static const size_t s_iNext[3] = {1, 2, 0};
        size_t i = 0;
        if (m.data[5] > m.data[0])
            i = 1;
        if (m.data[10] > m.data[i * 4 + i])
            i = 2;
        size_t j = s_iNext[i];
        size_t k = s_iNext[j];

        fRoot = Math::Sqrt(m.data[i * 4 + i] - m.data[j * 4 + j] - m.data[k * 4 + k] + 1.0f);
        q[i] = 0.5f * fRoot;
        fRoot = 0.5f / fRoot;
        q[3] = (m.data[j * 4 + k] - m.data[k * 4 + j]) * fRoot;
        q[j] = (m.data[i * 4 + j] + m.data[j * 4 + i]) * fRoot;
        q[k] = (m.data[i * 4 + k] + m.data[k * 4 + i]) * fRoot;
    }

    return q;
}

Vec4 Quaternion::ToAngleAxis() const {
    auto q = Vec4::Zero;
    auto sqr = data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
    if (sqr > 0) {
        auto inv = Math::InvSqrt(sqr);
        q[0] = 2.0f * Math::ArcCos(data[3]);
        q[1] = data[0] * inv;
        q[2] = data[1] * inv;
        q[3] = data[2] * inv;
    }
    return q;
}