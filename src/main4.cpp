
#include <fury/core.h>
#include <imgui/imgui.h>


struct Transform : public Component {
    Vec3 m_Position;
    f32 m_Speed;

    explicit Transform(f32 speed) : Component(),
                                    m_Position(Vec3::zero),
                                    m_Speed(speed) {}
};

struct TempSystem : public System<Transform> {
    Camera2d *m_Camera;

    TempSystem(Camera2d *camera) : m_Camera(camera) {};

    void Update() override {
        for (auto &compTuple: m_Components) {
            Transform *p_Transform = std::get<Transform *>(compTuple);
            p_Transform->m_Position = Vec3::lerp(p_Transform->m_Position, Vec3(m_Camera->ScreenToWorld()),
                                                 p_Transform->m_Speed * Time::DeltaTime);
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
            manager->AddSystem<TempSystem>(camera);

            entityId = manager->CreateEntity();
            manager->AddComponent<Transform>(entityId, 22);
            manager->AddEntity(entityId);


            entityId = manager->CreateEntity();
            manager->AddComponent<Transform>(entityId, 10);
            manager->AddEntity(entityId);


            entityId = manager->CreateEntity();
            manager->AddComponent<Transform>(entityId, 2);
            manager->AddEntity(entityId);


            manager->CreateSystems();
        }

        void Update() override {
            manager->Update();
            camera->Update();
            DebugDraw::SetCamera(camera->GetView(), camera->GetProjection());
        }

        void Render() override {
            DebugDraw::Pivot(Vec2::zero);
        }
    };
    return Win1().run();
}