
#include <fury/core.h>
#include <imgui/imgui.h>


struct Transform : public Component {
    Mat4 m_Transform;
    Vec3 m_Position;
    Quaternion m_Rotation;
    Vec3 m_Scale;

    Transform() : Component(),
                  m_Transform(Mat4::identify),
                  m_Position(Vec3::zero),
                  m_Rotation(Quaternion::identify),
                  m_Scale(Vec3::one) {};
};

struct Renderable : public Component {
    unsigned char *m_Texture;

    Renderable() : Component(), m_Texture(nullptr) {}

    ~Renderable() override {
        delete[] m_Texture;
        m_Texture = nullptr;
    }

    void Create() override {
        m_Texture = new unsigned char[512];
    }
};

struct RenderSystem : public System<Renderable, Transform> {
    explicit RenderSystem(EntityManager *manager) : System(manager) {}

    void Update() override {
        for (auto &compTuple: m_Components) {
            Transform *p_Transform = std::get<Transform *>(compTuple);
            Renderable *p_Renderable = std::get<Renderable *>(compTuple);
            p_Renderable->m_Texture[0] = static_cast<char>(p_Transform->m_Position.x());
        }
    }
};

struct TempSystem : public System<Transform> {
    explicit TempSystem(EntityManager *manager) : System(manager) {}

    void Update() override {
        for (auto &compTuple: m_Components) {
            Transform *p_Transform = std::get<Transform *>(compTuple);
            DebugDraw::Pivot(p_Transform->m_Position.xy());
        }
    }
};


int main() {
    class Win1 : public Window {
        Camera2d *camera = new Camera2d();
        EntityManager *manager;
        EntityId entityId;

        void Create() override {
            manager = new EntityManager();
            manager->AddSystem<RenderSystem, TempSystem>();
            entityId = manager->CreateEntity();
            manager->AddComponent<Transform>(entityId);
            manager->CreateSystems();
            manager->AddEntity(entityId);
        }

        void Update() override {
            manager->Update();
            camera->Update();
            DebugDraw::SetCamera(camera->GetView(), camera->GetProjection());
            if (Input::MouseDown(MOUSE_BUTTON_LEFT)) {
                auto p = manager->GetComponent<Transform>(entityId);
                p->m_Position = Vec3(camera->ScreenToWorld());
            }
        }

        void OnDrawGUI() override {
            bool open;
            ImGui::Begin("Another Window", &open);
            ImGui::Text("Framerate: %f", Time::ElapsedTime);
            ImGui::End();
        }

        void Render() override {
            DebugDraw::Pivot(Vec2::zero);
        }
    };
    return Win1().run();
}