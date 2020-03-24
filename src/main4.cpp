
#include <fury/core.h>
#include <imgui/imgui.h>

class Win1 : public Window {
    Camera2d *camera = Camera2d::instance();
    DebugDraw *draw = DebugDraw::instance();
    float zoom = 0;

    void init() override {
        add(camera);
        add(draw);
    }

    void create() override {
    }

    void update() override {
        auto targetPosition = camera->screenToWorld(input->position());
        draw->setCamera(camera->view, camera->projection);
    }

    void gui() override {
        bool open;
        ImGui::Begin("Another Window", &open);
        ImGui::Text("Hello from another window!");
        ImGui::SliderFloat("zoom", &zoom, 0, 10);
        ImGui::End();
    }

    void render() override {
        draw->pivot(Vec2::zero);
    }
};


int main() {
    Win1 w;
    return w.run();
}