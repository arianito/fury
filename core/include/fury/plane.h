//
// Created by aryan on 3/21/20.
//

#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include "vec3.h"
#include "ray.h"

struct Plane {

    Plane(const Vec3 &normal, const Vec3 &inPoint);

    Plane(const Vec3 &a, const Vec3 &b, const Vec3 &c);

    Plane(const Vec3 &normal, const f32& d);

    Vec3 closestPointOnPlane(const Vec3 &point);

    void setNormalAndPosition(const Vec3 &inNormal, const Vec3 &inPoint) ;

    void set3Points(const Vec3 &a, const Vec3 &b, const Vec3 &c) ;
    void flip() ;

    Plane flipped() ;
    void translate(const Vec3 &translation) ;

    static Plane translate(const Plane &plane, const Vec3 &translation);

    float getDistanceToPoint(const Vec3 &point);

    bool getSide(const Vec3 &point);

    bool sameSide(const Vec3 &inPt0, const Vec3 &inPt1);
    bool raycast(const Ray &ray, float *enter) ;


    [[nodiscard]] const Vec3 &normal() const ;

    [[nodiscard]] const f32 &distance() const;

private:
    Vec3 m_Normal;
    f32 m_Distance;

};
#endif //GAME_PLANE_H
