
#include <fury/core.h>

struct Bone {
    f32 angle;
    f32 len;
};

class Win1 : public Window {
    Mat4 view{0};
    Mat4 proj{0};
    DebugDraw draw;
    f32 camZoom = 0.5;
    f32 lastZoom = 0;
    Vec2 camPosition{0, 0};
    Vec2 lastMouse{0};
    Vec2 lastPos{0};

    void create() override {
        draw.create();

    }

    Vec2 convert(const Mat4 &m) {
        auto matInverse = m.invert();
        auto vIn = Vec4();
        vIn[0] = 2.0 * (input->x() / display->width) - 1.0f;
        vIn[1] = -2.0 * (input->y() / display->height) + 1.0f;
        vIn[2] = 1.0;
        vIn[3] = 1.0;
        auto pos = vIn * matInverse;
        pos[3] = 1.0 / pos[3];
        pos[0] *= pos[3];
        pos[1] *= pos[3];
        pos[2] *= pos[3];
        return {pos[0], pos[1]};
    }

    Vec2 convert() {
        return convert(proj * Mat4::createScale(Vec3(camZoom)));
    }

    void update() override {
        if (input->keyPress(KEY_LEFT_SHIFT)) {
            if (input->mouseDown(MOUSE_BUTTON_LEFT)) {
                lastMouse = convert();
                lastPos = camPosition;
            }
            if (input->mousePress(MOUSE_BUTTON_LEFT)) {
                auto pos = convert();
                camPosition = lastPos + (pos - lastMouse);
            }
            if (input->mouseDown(MOUSE_BUTTON_RIGHT)) {
                lastMouse = input->position();
                lastZoom = camZoom;
            }
            if (input->mousePress(MOUSE_BUTTON_RIGHT)) {
                auto pos = input->position();
                auto diff = (lastMouse.y() - pos.y()) / display->height * 2;
                camZoom = Math::clamp(lastZoom * (1 + diff) * (1 + diff), 0.0001, 10);
            }
        }
    }

    void render() override {
        f32 sz = 10;
        f32 height = sz / display->ratio();
        proj = Mat4::orthographic(height / 2, sz / 2, -height / 2, -sz / 2, -100, 100);
        view = Mat4::createTranslation(Vec3{camPosition.x(), camPosition.y(), 0} * camZoom) *
               Mat4::createScale(Vec3(camZoom));

        auto pos = convert(proj * view);

        f32 a = 0;
        if (pos.x() < 0)
            a = Math::arcTan(pos.y() / pos.x()) + Math::PI;
        else
            a = Math::arcTan(pos.y() / pos.x());

        f32 r2 = pos.sqrMagnitude();
        f32 a1 = 2;
        f32 a2 = 2.3;

        f32 q2 = Math::arcCos(Math::clamp((r2 - a1 * a1 - a2 * a2) / (2 * a1 * a2), -1, 1));
        f32 q1 = a - Math::arcTan((a2 * Math::sin(q2)) / (a1 + a2 * Math::cos(q2)));

        Bone bones[2];
        bones[0] = Bone{q1, a1};
        bones[1] = Bone{q2, a2};
        Vec2 from = {0, 0};
        f32 ang = 0;
        for (auto b : bones) {
            ang += b.angle;
            Vec2 next = {
                    from.x() + b.len * Math::cos(ang),
                    from.y() + b.len * Math::sin(ang),
            };
            draw.drawSegment(from, next, Color(1, 0, 0));
            draw.drawSolidCircle(from, 0.1, Vec2::up, Color(1, 0, 0));
            from = next;
        }
        draw.drawSolidCircle(from, 0.1, Vec2::up, Color(1, 0, 0));


        draw.update(view, proj);
        draw.render();
    }


    void dispose() override {
        draw.dispose();
    };
};


int main() {
    Win1 w;
    return w.run();
}