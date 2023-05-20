
#include <fury/core.h>

struct Player : public Component {
};

struct Projectile : public Component {
    Vec2 m_InitialPosition;
    Vec2 m_Position;
    f32 m_Rotation;
    f32 m_Speed;

    explicit Projectile(Vec2 pos, f32 rot, f32 speed) :
            Component(),
            m_Position(pos),
            m_InitialPosition(pos),
            m_Rotation(rot),
            m_Speed(speed) {}
};

struct Transform : public Component {
    Vec2 m_Position;
    f32 m_Rotation;

    explicit Transform() : Component(),
                           m_Position(Vec2::zero),
                           m_Rotation(0) {}
};

struct Renderable : public Component {
};

struct MovementSystem : public System<Transform> {
    void Update() override {
        auto vertical = Input::GetAxis(AXIS_VERTICAL);
        auto horizontal = Input::GetAxis(AXIS_HORIZONTAL);
        for (auto &compTuple: m_Components) {
            Transform *p_transform = std::get<Transform *>(compTuple);
            p_transform->m_Rotation = Math::lerp(p_transform->m_Rotation, p_transform->m_Rotation - horizontal,
                                                 Time::DeltaTime);
            auto angle = p_transform->m_Rotation;
            p_transform->m_Position = Vec2::lerp(p_transform->m_Position, p_transform->m_Position +
                                                                          Vec2(Math::cos(angle), Math::sin(angle)) *
                                                                          vertical,
                                                 Time::DeltaTime);
        }
    }
};

struct ProjectileSystem : public System<Projectile> {
    void Update() override {
        for (auto &compTuple: m_Components) {
            Projectile *p_projectile = std::get<Projectile *>(compTuple);

            auto angle = Vec2(Math::cos(p_projectile->m_Rotation), Math::sin(p_projectile->m_Rotation));
            p_projectile->m_Position = p_projectile->m_Position +
                                       angle *
                                       p_projectile->m_Speed *
                                       Time::DeltaTime;

            auto d = Vec2::distance(p_projectile->m_Position, p_projectile->m_InitialPosition) + 0.1f;
            if (d > 10) {
                m_EntityManager->DestroyEntity(p_projectile->GetEntityId());
            }

            DebugDraw::Segment(
                    p_projectile->m_Position,
                    p_projectile->m_Position + angle * 0.05f,
                    Color::red);
        }
    }
};

struct ShooterSystem : public System<Player, Transform> {
    f32 last = 0;

    void Update() override {
        auto down = Input::KeyPress(KEY_SPACE);
        if (down && Time::ElapsedTime - last > 0.05f) {
            for (auto &compTuple: m_Components) {
                Player *p_player = std::get<Player *>(compTuple);
                Transform *p_transform = std::get<Transform *>(compTuple);
                auto manager = m_EntityManager;
                auto entityId = manager->CreateEntity();
                manager->AddComponent<Projectile>(entityId, p_transform->m_Position, p_transform->m_Rotation, 20.0f);
            }
            last = Time::ElapsedTime;
        }
    }
};

struct RenderSystem : public System<Transform, Renderable> {
    void Update() override {
        auto axis = Input::GetAxis(AXIS_VERTICAL);
        for (auto &compTuple: m_Components) {
            Transform *p_player = std::get<Transform *>(compTuple);
            auto angle = p_player->m_Rotation;
            DebugDraw::Circle(p_player->m_Position, 0.1f, Color::red);
            DebugDraw::Segment(
                    p_player->m_Position,
                    p_player->m_Position + Vec2(Math::cos(angle), Math::sin(angle)) * 0.1f,
                    Color::red
            );
        }
    }
};


int main() {
    class Win1 : public Window {
        Camera2d *camera = new Camera2d();
        EntityManager manager;

        void Create() override {
            manager.AddSystem<MovementSystem>();
            manager.AddSystem<RenderSystem>();
            manager.AddSystem<ProjectileSystem>();
            manager.AddSystem<ShooterSystem>();

            auto entityId = manager.CreateEntity();
            manager.AddComponent<Player>(entityId);
            manager.AddComponent<Transform>(entityId);
            manager.AddComponent<Renderable>(entityId);

            manager.CreateSystems();
        }

        void OnDrawGUI() override {
            bool open = true;
            ImGui::Begin("Hello", &open);
            ImGui::Text("framerate: %f", 1.0f / Time::DeltaTime);
            ImGui::Text("System: %zu / %zu", manager.m_SystemAllocator->GetUsedMemory(),
                        manager.m_SystemAllocator->GetMemorySize());
            ImGui::Text("%s: %zu / %zu",
                        manager.m_EntityContainer->GetEntityContainerTypeName(),
                        manager.m_EntityContainer->GetUsedMemory(),
                        manager.m_EntityContainer->GetMemorySize());
            for (auto &a: manager.m_ComponentContainerRegistry) {
                auto b = a.second;

                ImGui::Text("%s: %zu / %zu",
                            b->GetComponentContainerTypeName(),
                            b->GetUsedMemory(),
                            b->GetMemorySize());

            }
            ImGui::End();
        }

        void Update() override {
            manager.Update();
            camera->Update();
            DebugDraw::SetCamera(camera->GetView(), camera->GetProjection());
        }

        void Render() override {
            DebugDraw::Pivot(Vec2::zero);
        }
    };
    return Win1().run();
}