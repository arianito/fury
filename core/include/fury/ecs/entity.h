#pragma once

#include "ecs_types.h"
#include "component.h"
#include <unordered_map>

#define ENTITY_LUT_GROW                        1024

#define ENTITY_T_CHUNK_SIZE                    512


struct Entity {
    using ComponentMap = std::unordered_map<ComponentId, Component *>;
    EntityId m_Id;
    ComponentMap m_Components;
public:
    explicit Entity(EntityId p_Id) : m_Id(p_Id) {}

    Entity(const Entity &) = delete;

    Entity(Entity &&) = default;

    Entity &operator=(const Entity &) = delete;

    Entity &operator=(Entity &&) = default;

    ~Entity() = default;

    EntityId GetId() const;

    template<class T>
    void AddComponent(T *pComponent);

    const ComponentMap &GetComponents() const;

    template<class T>
    T *GetComponent() const;
};

template<class T>
void Entity::AddComponent(T *pComponent) {
    auto id = GetComponentTypeID<T>();
    assert(m_Components.find(id) == m_Components.end());
    m_Components.emplace(id, pComponent);
}

template<class T>
T *Entity::GetComponent() const {
    auto found = m_Components.find(GetComponentTypeID<T>());
    if (found != m_Components.end()) {
        return (T *) found->second;
    } else {
        return nullptr;
    }
}
