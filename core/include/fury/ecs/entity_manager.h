#pragma once

#include "ecs_types.h"
#include "component.h"
#include "base_system.h"
#include "entity.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <fury/memory/linear_allocator.h>
#include <fury/memory/global_memory_user.h>

#define ECS_SYSTEM_MEMORY_BUFFER_SIZE 8388608

struct EntityManager : public GlobalMemoryUser {
private:
    size_t m_EntityCnt = 0;
    using SystemAllocator    = LinearAllocator;
    using SystemVector = std::unordered_map<SystemId, BaseSystem *>;
    using EntityMap = std::unordered_map<EntityId, Entity *>;
    using ComponentMap = std::unordered_map<ComponentId, std::unordered_map<EntityId, Component *>>;
public:
    SystemVector m_Systems;
    EntityMap m_Entities;
    ComponentMap m_Components;

    SystemAllocator *m_SystemAllocator;

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

    template<class T, class ...Args>
    T *AddComponent(EntityId entityId, Args &&... args);

    template<class T, class ...Args>
    T *AddSystem(Args &&... args);

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
    auto findIt = m_Components.find(id);
    if (findIt != m_Components.end()) {
        auto it = findIt->second.find(entityId);
        if (it != findIt->second.end() && it->second != nullptr) {
            return (T *) it->second;
        }
    }
    T *component = new T(std::forward<Args>(args)...);
    component->SetEntityId(entityId);
    if (findIt == m_Components.end()) {
        auto mp = std::unordered_map<EntityId, Component *>();
        mp.emplace(entityId, component);
        m_Components[id] = mp;
    } else {
        findIt->second[entityId] = component;
    }
    entity->second->AddComponent<T>(component);
    return component;
}


template<class T, class... Args>
T *EntityManager::AddSystem(Args &&... args) {
    auto id = GetSystemTypeID<T>();
    auto it = m_Systems.find(id);
    if (it != m_Systems.end() && it->second != nullptr) {
        return (T *) it->second;
    }
    T *system = nullptr;
    void *pSystemMem = this->m_SystemAllocator->Allocate(sizeof(T), alignof(T));
    if (pSystemMem != nullptr) {
        ((T*)pSystemMem)->m_EntityManager = this;
        system = new (pSystemMem)T(std::forward<Args>(args)...);
        this->m_Systems[id] = system;
        log_info("System \'%s\' (%d bytes) created.", typeid(T).name(), sizeof(T));
    } else {
        log_fatal("Unable to create system \'%s\' (%d bytes).", typeid(T).name(), sizeof(T));
    }
    return system;

}
