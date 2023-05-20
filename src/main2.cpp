
#include <fury/core.h>

struct Effector {
    Vec3 position;
};
struct Joint {
    Vec3 position{};
    Effector *effector = nullptr;
};

struct Skeleton {
    Vector<Joint *> joints{};
    f32 snapBackStrength = 1.0f;

    void add(Joint *j) {
        joints.emplace_back(j);
    }

    void init() {
        m_BoneLengths.clear();
        f32 acc = 0;
        m_Len = joints.size();
        for (int i = 0; i < m_Len - 1; i++) {
            auto j1 = joints.at(i);
            auto j2 = joints.at(i + 1);
            auto direction = j2->position - j1->position;
            m_StartDirections.emplace_back(direction);
            f32 distance = direction.magnitude();
            m_BoneLengths.emplace_back(distance);
            acc += distance;
        }
        m_TotalLength = acc;
    }


    void resolve(const Vec3 &targetPosition) {
        for (int i = 0; i < m_Len - 1; i++) {
            auto j1 = joints.at(i);
            auto j2 = joints.at(i + 1);
            j2->position = Vec3::lerp(j2->position, j1->position + m_StartDirections.at(i), snapBackStrength);
        }

        for (int iteration = 0; iteration < 2; iteration++) {
            for (int i = m_Len - 1; i > 0; i--) {

                auto j0 = joints.at(i - 1);
                auto j1 = joints.at(i);
                auto j2 = joints.at(i + 1);

                if (i == m_Len - 1)
                    j1->position = targetPosition; //set it to target
                else
                    Positions[i] = Positions[i + 1] + (Positions[i] - Positions[i + 1]).normalized *
                                                      BonesLength[i]; //set in line on distance
            }

            //forward
            for (int i = 1; i < Positions.Length; i++)
                Positions[i] = Positions[i - 1] + (Positions[i] - Positions[i - 1]).normalized * BonesLength[i - 1];

            //close enough?
            if ((Positions[Positions.Length - 1] - targetPosition).sqrMagnitude < Delta * Delta)
                break;
        }


    }

private:
    Vector<f32> m_BoneLengths{};
    Vector<Vec3> m_StartDirections{};
    f32 m_TotalLength{};
    int m_Len{};
};


struct Temp : public Entity {
    enum type {
        length = 10,
    };
    Vec2 joints[length + 1];
    f32 boneLengths[length]{};
    Vec2 targetPosition{0, 10};

    Effector eff{Vec3{0, 0}};
    Effector eff2{Vec3{0, 0}};
    Skeleton sk;

    void create() override {
        f32 acc = 0;
        for (int i = 0; i < length + 1; i++) {
            boneLengths[i] = (i + 1) * 0.2;
            joints[i] = Vec2(0, acc);
            if (i < length) {
                acc += boneLengths[i];
            }
        }

        Joint j1{Vec3{0, 0}, &eff};
        Joint j2{Vec3{1, 0}};
        Joint j3{Vec3{2, 0}};
        Joint j4{Vec3{3, 0}, &eff2};
        sk.add(&j1);
        sk.add(&j2);
        sk.add(&j3);
        sk.add(&j4);
        sk.init()
    }

    void render() override {
        for (int i = length; i > 0; i--) {
            if (i == length)
                joints[i] = targetPosition;
            else
                joints[i] = joints[i + 1] + (joints[i] - joints[i + 1]).normal() * boneLengths[i];
        }
        for (int i = 1; i < length + 1; i++)
            joints[i] = joints[i - 1] + (joints[i] - joints[i - 1]).normal() * boneLengths[i - 1];

    }
};


class Win1 : public Window {
    DebugDraw draw;
    Temp skeleton;
    Camera2d camera;

    void init() override {
        add(&draw);
        add(&skeleton);
        add(&camera);
    }

    void update() override {
        auto targetPosition = camera.screenToWorld(input->position());
        draw.setCamera(camera.view, camera.projection);
        skeleton.targetPosition = targetPosition;
        if (input->mouseDown(MOUSE_BUTTON_LEFT)) {
            if (skeleton.isActive()) {
                skeleton.deactivate();
            } else {
                skeleton.activate();
            }
        }
    }

    void render() override {
        for (int i = 0; i < skeleton.length; i++) {
            auto inv = (f32) (skeleton.length - i);
            auto a = skeleton.joints[i];
            auto b = skeleton.joints[i + 1];
            draw.segment(a, b, Color(0.5, 0.6, 0.6, Math::clamp01(inv * 0.1)));
            auto radius = Math::clamp(inv * 0.02f * skeleton.boneLengths[0], 0.02f, 1);
            draw.circle(a, radius, Color(0.5, 0.6, 0.6, Math::clamp01(inv * 0.1)));
        }

        draw.pivot(Vec2::zero);
    }
};


int main() {
    Win1 w;
    return w.run();
}