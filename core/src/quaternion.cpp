#include "fury/quaternion.h"
#include "fury/mathf.h"


const Quaternion Quaternion::identify{0.0f, 0.0f, 0.0f, 1.0f};


const Quaternion Quaternion::fromAngleAxis(const f32 &angle, const Vec3 &axis) {
    auto ha = 0.5 * angle;
    auto sn = Math::sin(ha);

    return Quaternion{
            sn * axis[0],
            sn * axis[1],
            sn * axis[2],
            Math::cos(ha)
    };
}

const Quaternion Quaternion::fromEulerAngles(const f32 &pitch, const f32 &roll, const f32 &yaw) {
    f32 cy = Math::cos(pitch * 0.5f);
    f32 sy = Math::sin(pitch * 0.5f);
    f32 cr = Math::cos(roll * 0.5f);
    f32 sr = Math::sin(roll * 0.5f);
    f32 cp = Math::cos(yaw * 0.5f);
    f32 sp = Math::sin(yaw * 0.5f);

    return Quaternion{
            cy * cr * cp + sy * sr * sp,
            cy * sr * cp - sy * cr * sp,
            cy * cr * sp + sy * sr * cp,
            sy * cr * cp - cy * sr * sp,
    };
}

const Quaternion Quaternion::fromMatrix(const Uniform<4, 4> &m) {
    auto q = Quaternion::identify;
    f32 fTrace = m.data[0] + m.data[5] + m.data[10];
    f32 fRoot;
    if (fTrace > 0.0) {
        fRoot = Math::sqrt(fTrace + 1.0f);  // 2w
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

        fRoot = Math::sqrt(m.data[i * 4 + i] - m.data[j * 4 + j] - m.data[k * 4 + k] + 1.0f);
        q[i] = 0.5f * fRoot;
        fRoot = 0.5f / fRoot;
        q[3] = (m.data[j * 4 + k] - m.data[k * 4 + j]) * fRoot;
        q[j] = (m.data[i * 4 + j] + m.data[j * 4 + i]) * fRoot;
        q[k] = (m.data[i * 4 + k] + m.data[k * 4 + i]) * fRoot;
    }

    return q;
}

Vec4 Quaternion::toAngleAxis() const {
    auto q = Vec4::zero;
    auto sqr = data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
    if (sqr > 0) {
        auto inv = Math::invSqrt(sqr);
        q[0] = 2.0f * Math::arcCos(data[3]);
        q[1] = data[0] * inv;
        q[2] = data[1] * inv;
        q[3] = data[2] * inv;
    }
    return q;
}