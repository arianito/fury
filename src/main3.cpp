
#include <fury/core.h>

struct Effector {
    Vec3 position;
};
struct Joint {
    Vec3 position{};
    Effector *effector = nullptr;
};

struct Skeleton {
    Effector* pole = nullptr;
    Vector<Joint *> joints;
    f32 snapBackStrength = 0.4f;

    void add(Joint *j) {
        joints.emplace_back(j);
    }

    void init() {
        f32 acc = 0;
        m_Len = joints.size();
        for (int i = 0; i < m_Len - 1; i++) {
            auto j1 = joints.at(i);
            auto j2 = joints.at(i + 1);
            auto direction = j2->position - j1->position;
            m_StartDirections.push_back(direction);
            f32 distance = direction.magnitude();
            m_BoneLengths.push_back(distance);
            acc += distance;
        }
        m_TotalLength = acc;
    }

    Joint *at(int i) {
        return joints.at(i);
    }

    [[nodiscard]] const int &size() const {
        return m_Len;
    }


    void resolve() {
        f32 ang = 0;
        for (int i = 0; i < m_Len - 1; i++) {
            auto len = m_BoneLengths.at(i);
            if (i == 0) {
                auto d = m_StartDirections.at(i);
                auto j1 = joints.at(i);
                auto j2 = joints.at(i + 1);
                ang = -Vec3::signedAngle(j2->position - j1->position, d, Vec3::forward);
                // j2->position = Vec3::lerp(j2->position, j1->position + m_StartDirections.at(i), snapBackStrength);
            } else {
                auto j0 = joints.at(i - 1);
                auto j1 = joints.at(i);
                auto j2 = joints.at(i + 1);
                auto angle = Vec3::signedAngle(j1->position - j0->position, j2->position - j1->position, Vec3::forward);
                
                j2->position = Vec3::lerp(j2->position, j1->position + Vec3(Math::cos(ang), Math::sin(ang), 0) * len, snapBackStrength);
                ang += angle;
            }
        }

        for (int iteration = 0; iteration < 3; iteration++) {
            for (int i = m_Len - 1; i >= 0; i--) {
                auto j1 = joints.at(i);
                if (j1->effector != nullptr) {
                    j1->position = j1->effector->position;
                } else {
                    if (i < m_Len - 1) {
                        auto j2 = joints.at(i + 1);
                        j1->position = Vec3::lerp(j1->position, j2->position + (j1->position - j2->position).normal() * m_BoneLengths.at(i), snapBackStrength);
                    }
                }
            }
            for (int i = 1; i < m_Len; i++) {
                auto j0 = joints.at(i - 1);
                auto j1 = joints.at(i);
                j1->position = Vec3::lerp(j1->position, j0->position + (j1->position - j0->position).normal() * m_BoneLengths.at(i - 1), snapBackStrength);
            }
        }

        for (int i = 1; i < m_Len - 1; i++) {
            auto j0 = joints.at(i - 1);
            auto j1 = joints.at(i);
            auto j2 = joints.at(i + 1);

            auto h = (j2->position - j0->position) / 2.0f;
            auto plane = Plane(h, j0->position);

            auto p =  h + Vec3(-h.y(), h.x(), 0) * 4.0f;

            auto projectedPole = plane.closestPointOnPlane(p);
            auto projectedBone = plane.closestPointOnPlane(j1->position);
            auto angle = Vec3::signedAngle(projectedBone - j0->position, projectedPole - j0->position, plane.normal());
            j1->position = Quaternion::fromAngleAxis(angle, plane.normal()) * (j1->position - j0->position) + j0->position;
        }
    }

    Vector<f32> m_BoneLengths;
    Vector<Vec3> m_StartDirections;
private:
    f32 m_TotalLength{};
    int m_Len{};
};


struct Body1 : public Entity {
    Body1(DebugDraw *pDraw) {
        draw = pDraw;
    }

    DebugDraw *draw;

    void create() override {
        sk.add(new Joint{Vec3{0, 0}, new Effector{Vec3{0, 0}}});
        sk.add(new Joint{Vec3{0.5, 0}});
        sk.add(new Joint{Vec3{1.1, 0}, new Effector{Vec3{0.95, 0}}});
        sk.init();
    }
    void setHip(const Vec3& pos) {
        sk.joints.at(0)->effector->position = pos;
    }

    void setLeg(const Vec3& pos) {
        sk.joints.at(sk.size() - 1)->effector->position = pos;
    }


    void setPole(Effector *effector) {
        sk.pole = effector;
    }

    bool a = false;
    void render() override {
        sk.resolve();
        f32 ang = 0;
        for (int i = 0; i < sk.size(); i++) {
            auto a = sk.at(i);
            if (i < sk.size() - 1) {
                auto b = sk.at(i + 1);
                draw->segment(a->position.xy(), b->position.xy(), Color::yellow);

                if (i == 0) {
                    auto d = sk.m_StartDirections.at(i);
                    auto j0 = sk.joints.at(i);
                    auto j1 = sk.joints.at(i + 1);
                    auto len = sk.m_BoneLengths.at(i);
                    ang = -Vec3::signedAngle(j1->position - j0->position, d, Vec3::forward);
                    // draw->segment(j1->position.xy(), (j1->position + Vec3(Math::cos(ang), Math::sin(ang), 0) * len).xy(), Color::black);
                } else {
                    auto j0 = sk.joints.at(i - 1);
                    auto j1 = sk.joints.at(i);
                    auto j2 = sk.joints.at(i + 1);
                    auto len = sk.m_BoneLengths.at(i);
                    auto angle = Vec3::signedAngle(j1->position - j0->position, j2->position - j1->position, Vec3::forward);
                    draw->segment(j0->position.xy(), j2->position.xy(), Color(1,1,1,0.3));
                    ang += angle;
                }
            }
            draw->circle(a->position.xy(), 0.05, Color::yellow);
            if (a->effector != nullptr) {
                draw->circle(a->effector->position.xy(), 0.02, Color::red);
            }
        }
    }

private:
    Skeleton sk;
};


class Win1 : public Window {
    DebugDraw draw;
    Body1 body1{&draw};
    Body1 body2{&draw};
    Camera2d *camera;

    void init() override {
        camera = Camera2d::instance();
        add(camera);
        add(&draw);
        add(&body1);
        add(&body2);
    }

    f32 a = 0;
    Vec3 lega{0, -1};
    Vec3 legb{0, -1};
    Vec3 legc{0, -1};
    Vec3 legd{0, -1};
    Vec3 hip{0, 0};
    void update() override {
        auto targetPosition = camera->screenToWorld(input->position());
        draw.setCamera(camera->view, camera->projection);

        auto p = -input->getAxis(1);
        bool run = input->keyPress(KEY_LEFT_SHIFT);
        if(Math::abs(p) < 0.1f) {
            lega = Vec3::lerp(lega, Vec3(-0.1f, -0.95), 0.1f);
            legb = Vec3::lerp(legb, Vec3(0.2f, -0.95), 0.1f);
            hip = Vec3::lerp(hip, Vec3(0, 0), 0.1f);
        } else{

            if(run)
                a += time->deltaTime * 8 * p;
            else
                a += time->deltaTime * 4 * p;

            if (a < -2*Math::PI) {
                a += 2*Math::PI;
            }else if(a > 2*Math::PI) {
                a -= 2*Math::PI;
            }

            auto theta1 = 0.0f;
            auto theta2 = 0.0f;
            auto theta3 = 0.0f;
            auto ws = 0.5f;
            auto hs = 0.3f;

            if(run){
                theta1 = a + 0.5f * 2 * Math::PI;
                theta2 = a + 1.0f * 2 * Math::PI;
                theta3 = a * 2.0f;
            }else{
                auto t0 = a + 0.5f * 2 * Math::PI;
                auto t1 = a + 1.0f * 2 * Math::PI;
                auto t2 = a;
                theta1 = t0 + Math::ramp(-Math::cos(t0)) * Math::sin(t0);
                theta2 = t1 + Math::ramp(-Math::cos(t1)) * Math::sin(t1);
                theta3 = t2 * 2.0f;
            }


            if(run)
                lega = Vec3{Math::cos(theta1) * ws - ws/2.0f, Math::sin(theta1) * hs, 0};
            else
                lega = Vec3{Math::cos(theta1) * ws, Math::sin(theta1) * hs, 0};

            if(lega[1] < 0)
                lega[1] = 0;
            lega[1] -= 0.95;
            if(run)
                legb = Vec3{Math::cos(theta2) * ws - ws/2.0f, Math::sin(theta2) * hs, 0};
            else
                legb = Vec3{Math::cos(theta2) * ws, Math::sin(theta2) * hs, 0};
            if(legb[1] < 0)
                legb[1] = 0;
            legb[1] -= 0.95;

            hip = Vec3{Math::cos(theta3)*0.05f, Math::sin(theta3)*0.1f};
            if(run)
                hip = hip + Vec3(0.2f, -0.2f);

        }

        body1.setLeg(lega);
        body2.setLeg(legb);

        body1.setHip(hip+Vec3(-0.2f, 0));
        body2.setHip(hip+Vec3(0.0f, 0));
    }

    void render() override {
        draw.pivot(Vec2::zero);
    }
};


int main() {
    Win1 w;
    return w.run();
}