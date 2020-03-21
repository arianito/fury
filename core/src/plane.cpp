//
// Created by aryan on 3/21/20.
//

#include "fury/plane.h"

Plane::Plane(const Vec3 &normal, const Vec3 &inPoint) {

    m_Normal = Vec3::normalize(normal);
    m_Distance = -Vec3::dot(m_Normal, inPoint);
}

Plane::Plane(const Vec3 &a, const Vec3 &b, const Vec3 &c) {

    m_Normal = Vec3::normalize(Vec3::cross(b - a, c - a));
    m_Distance = -Vec3::dot(m_Normal, a);
}

Plane::Plane(const Vec3 &normal, const f32 &d) {

    m_Normal = Vec3::normalize(normal);
    m_Distance = d;
}

Vec3 Plane::closestPointOnPlane(const Vec3 &point) {
    auto pointToPlaneDistance = Vec3::dot(m_Normal, point) + m_Distance;
    return point - (m_Normal * pointToPlaneDistance);
}

void Plane::setNormalAndPosition(const Vec3 &inNormal, const Vec3 &inPoint) {

    m_Normal = Vec3::normalize(inNormal);
    m_Distance = -Vec3::dot(inNormal, inPoint);
}

void Plane::set3Points(const Vec3 &a, const Vec3 &b, const Vec3 &c) {

    m_Normal = Vec3::normalize(Vec3::cross(b - a, c - a));
    m_Distance = -Vec3::dot(m_Normal, a);
}

void Plane::flip() {
    m_Normal = -m_Normal;
    m_Distance = -m_Distance;

}

Plane Plane::flipped() {
    return Plane(-m_Normal, -m_Distance);
}

void Plane::translate(const Vec3 &translation) {

    m_Distance += Vec3::dot(m_Normal, translation);
}

Plane Plane::translate(const Plane &plane, const Vec3 &translation) {
    return Plane(plane.m_Normal, plane.m_Distance + Vec3::dot(plane.m_Normal, translation));
}

float Plane::getDistanceToPoint(const Vec3 &point) {
    return Vec3::dot(m_Normal, point) + m_Distance;
}

bool Plane::getSide(const Vec3 &point) {
    return Vec3::dot(m_Normal, point) + m_Distance > 0.0F;
}

bool Plane::sameSide(const Vec3 &inPt0, const Vec3 &inPt1) {
    float d0 = getDistanceToPoint(inPt0);
    float d1 = getDistanceToPoint(inPt1);
    return (d0 > 0.0f && d1 > 0.0f) ||
           (d0 <= 0.0f && d1 <= 0.0f);
}

bool Plane::raycast(const Ray &ray, float *enter) {
    float vdot = Vec3::dot(ray.direction(), m_Normal);
    float ndot = -Vec3::dot(ray.origin(), m_Normal) - m_Distance;

    if (Math::approximately(vdot, 0.0f)) {
        *enter = 0.0F;
        return false;
    }
    *enter = ndot / vdot;
    return *enter > 0.0F;
}

const Vec3 &Plane::normal() const {
    return m_Normal;
}

const f32 &Plane::distance() const {
    return m_Distance;
}
