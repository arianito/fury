#pragma once

#include "ecs_types.h"
#include "component.h"
#include "base_system.h"
#include "entity.h"
#include <vector>
#include <iostream>
#include <cassert>

struct EntityManager {
private:
    size_t m_EntityCnt = 0;
    using SystemVector = std::vector<BaseSystem *>;
    using EntityMap = std::map<EntityId, Entity *>;
    using ComponentMap = std::map<ComponentId, std::vector<Component *>>;
public:
    SystemVector m_Systems;
    EntityMap m_Entities;
    ComponentMap m_Components;

    EntityManager(const EntityManager &) = delete;

    EntityManager &operator=(const EntityManager &) = delete;

    EntityManager(EntityManager &&) = delete;

    EntityManager &operator=(EntityManager &&) = delete;

    EntityManager();

    ~EntityManager();

    void CreateSystems();

    void Update();

    EntityId CreateEntity();

    void AddEntity(EntityId entityId);

    void DestroyEntity(EntityId entityId);

    template<class T>
    T *GetComponent(EntityId entityId) const;

    template<class...Args>
    void AddSystem();


    template<class T, class ...Args>
    T *AddComponent(EntityId entityId, Args &&... args);

private:
    template<size_t Index, class T, class...Args>
    void AddSystem();

    template<size_t Index>
    void AddSystem();
};

template<class T>
T *EntityManager::GetComponent(EntityId entityId) const {
    auto entity = m_Entities.find(entityId);
    assert (entity != m_Entities.end());
    return entity->second->GetComponent<T>();
}

template<class T, class... Args>
T *EntityManager::AddComponent(EntityId entityId, Args &&... args) {
    auto entity = m_Entities.find(entityId);
    assert (entity != m_Entities.end());

    auto id = GetComponentTypeID<T>();
    T *component = new T(std::forward<Args>(args)...);
    auto findIt = m_Components.find(id);
    if (findIt == m_Components.end()) {
        m_Components.emplace(id, std::vector<Component*>{component});
        component->SetEntityId(entityId);
        component->SetPoolId(0);
    } else {
        findIt->second.emplace_back(component);
        component->SetEntityId(entityId);
        component->SetPoolId(findIt->second.size() - 1);
    }
    entity->second->AddComponent<T>(component);
    return component;
}

template<size_t Index, class T, class... Args>
void EntityManager::AddSystem() {
    auto id = GetSystemTypeID<T>();
    AddSystem<Index + 1, Args...>();
    m_Systems.emplace_back(new T(this));
}

template<size_t Index>
void EntityManager::AddSystem() {
}

template<class... Args>
void EntityManager::AddSystem() {
    AddSystem<0, Args...>();
}