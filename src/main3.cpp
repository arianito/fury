
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
    f32 snapBackStrength = 0.1f;

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
        for (int i = 0; i < m_Len - 1; i++) {
            auto j1 = joints.at(i);
            auto j2 = joints.at(i + 1);
            j2->position = Vec3::lerp(j2->position, j1->position + m_StartDirections.at(i), snapBackStrength);
        }

        for (int iteration = 0; iteration < 3; iteration++) {
            for (int i = m_Len - 1; i >= 0; i--) {
                auto j1 = joints.at(i);
                if (j1->effector != nullptr) {
                    j1->position = j1->effector->position;
                } else {
                    if (i < m_Len - 1) {
                        auto j2 = joints.at(i + 1);
                        j1->position = j2->position + (j1->position - j2->position).normal() * m_BoneLengths.at(i);
                    }
                }
            }
            for (int i = 1; i < m_Len; i++) {
                auto j0 = joints.at(i - 1);
                auto j1 = joints.at(i);
                j1->position = j0->position + (j1->position - j0->position).normal() * m_BoneLengths.at(i - 1);
            }
        }

if (pole != nullptr) {
        for (int i = 1; i < m_Len - 1; i++) {
            auto j0 = joints.at(i - 1);
            auto j1 = joints.at(i);
            auto j2 = joints.at(i + 1);
            auto plane = Plane(j2->position - j0->position, j0->position);
            auto projectedPole = plane.closestPointOnPlane(pole->position);
            auto projectedBone = plane.closestPointOnPlane(j1->position);
            auto angle = Vec3::signedAngle(projectedBone - j0->position, projectedPole - j0->position,
                                           plane.normal());
            j1->position =
                    Quaternion::fromAngleAxis(angle, plane.normal()) * (j1->position - j0->position) + j0->position;
        }
        }
    }

private:
    Vector<f32> m_BoneLengths;
    Vector<Vec3> m_StartDirections;
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
        sk.add(new Joint{Vec3{1, 0}});
        sk.init();
    }

    void setEffector(Effector *effector) {
        sk.joints.at(2)->effector = effector;
    }

    void render() override {
        sk.resolve();
        for (int i = 0; i < sk.size(); i++) {
            auto a = sk.at(i);
            if (i < sk.size() - 1) {
                auto b = sk.at(i + 1);
                draw->segment(a->position.xy(), b->position.xy(), Color::yellow);
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
    Camera2d *camera;

    Effector *eff1 = new Effector();

    void init() override {
        camera = Camera2d::instance();
        add(camera);
        add(&draw);
        add(&body1);
    }

    void update() override {
        auto targetPosition = camera->screenToWorld(input->position());
        draw.setCamera(camera->view, camera->projection);

        eff1->position = Vec3(targetPosition);

        if (input->keyDown(KEY_A)) {
            body1.setEffector(eff1);
        }
        if (input->keyDown(KEY_D)) {
            body1.setEffector(nullptr);
        }
    }

    void render() override {
        draw.pivot(Vec2::zero);
    }
};


int main() {
    Win1 w;
    return w.run();
}