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
#include "fury/memory/memory_chunk_allocator.h"

#define COMPONENT_LUT_GROW                    1024
#define COMPONENT_T_CHUNK_SIZE                512
#define ECS_SYSTEM_MEMORY_BUFFER_SIZE 8388608
#define ENTITY_LUT_GROW                        1024
#define ENTITY_T_CHUNK_SIZE                    512


struct EntityManager : public GlobalMemoryUser {

    class IComponentContainer {
    public:
        virtual ~IComponentContainer() {}

        virtual const char *GetComponentContainerTypeName() const = 0;

        virtual void DestroyComponent(Component *object) = 0;

        virtual size_t GetUsedMemory() = 0;
        virtual size_t GetMemorySize() = 0;
    };

    template<class T>
    class ComponentContainer : public MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>, public IComponentContainer {
    public:
        ComponentContainer() : MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>("ComponentManager") {}

        virtual ~ComponentContainer() {}

        ComponentContainer(const ComponentContainer &) = delete;

        ComponentContainer &operator=(const ComponentContainer &) = delete;

        ComponentContainer(ComponentContainer &&) = delete;

        ComponentContainer &operator=(ComponentContainer &&) = delete;

        virtual const char *GetComponentContainerTypeName() const override {
            static const char *COMPONENT_TYPE_NAME{typeid(T).name()};
            return COMPONENT_TYPE_NAME;
        }

        virtual void DestroyComponent(Component *object) override {
            object->~Component();
            this->DestroyObject(object);
        }

        size_t GetUsedMemory() override {

            auto b = 0;
            for (auto chunk : this->m_Chunks) {
                b += chunk->allocator->GetUsedMemory();
            }
            return b;

        }
        size_t GetMemorySize() override {
            auto b = 0;
            for (auto chunk : this->m_Chunks) {
                b += chunk->allocator->GetMemorySize();
            }
            return b;
        }
    };

    class IEntityContainer {
    public:

        virtual ~IEntityContainer() {}

        virtual const char *GetEntityContainerTypeName() const = 0;

        virtual void DestroyEntity(Entity *object) = 0;
        virtual size_t GetUsedMemory() = 0;
        virtual size_t GetMemorySize() = 0;

    };

    template<class T>
    class EntityContainer : public MemoryChunkAllocator<T, ENTITY_T_CHUNK_SIZE>, public IEntityContainer {

        EntityContainer(const EntityContainer &) = delete;

        EntityContainer &operator=(EntityContainer &) = delete;

    public:

        EntityContainer() : MemoryChunkAllocator<T, ENTITY_T_CHUNK_SIZE>("EntityManager") {}

        virtual ~EntityContainer() {}

        virtual const char *GetEntityContainerTypeName() const override {
            static const char *ENTITY_TYPE_NAME{typeid(T).name()};
            return ENTITY_TYPE_NAME;
        }

        virtual void DestroyEntity(Entity *object) override {
            object->~Entity();
            this->DestroyObject(object);
        }

        size_t GetUsedMemory() override {

            auto b = 0;
            for (auto chunk : this->m_Chunks) {
                b += chunk->allocator->GetUsedMemory();
            }
            return b;

        }
        size_t GetMemorySize() override {
            auto b = 0;
            for (auto chunk : this->m_Chunks) {
                b += chunk->allocator->GetMemorySize();
            }
            return b;
        }
    };

private:
    size_t m_EntityCnt = 0;
    using SystemAllocator    = LinearAllocator;
    using SystemVector = std::unordered_map<SystemId, BaseSystem *>;
    using EntityMap = std::unordered_map<EntityId, Entity *>;
    using ComponentMap = std::unordered_map<ComponentId, std::unordered_map<EntityId, Component *>>;
    using ComponentContainerRegistry = std::unordered_map<ComponentId, IComponentContainer *>;

public:
    SystemVector m_Systems;
    EntityMap m_Entities;
    ComponentMap m_Components;

    SystemAllocator *m_SystemAllocator;
    ComponentContainerRegistry m_ComponentContainerRegistry;
    EntityContainer<Entity> *m_EntityContainer;


    EntityManager(const EntityManager &) = delete;

    EntityManager &operator=(const EntityManager &) = delete;

    EntityManager(EntityManager &&) = delete;

    EntityManager &operator=(EntityManager &&) = delete;

    EntityManager();

    ~EntityManager();

    void CreateSystems();

    void Update();

    EntityId CreateEntity();

    void DestroyEntity(EntityId entityId);


    EntityContainer<Entity> *GetEntityContainer();

    template<class T>
    ComponentContainer<T> *GetComponentContainer();

    template<class T>
    T *GetComponent(EntityId entityId) const;

    void RemoveAllComponent(EntityId entityId) const;

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
    void *pObjectMemory = GetComponentContainer<T>()->CreateObject();
    T *component = new(pObjectMemory)T(std::forward<Args>(args)...);
    component->SetEntityId(entityId);
    if (findIt == m_Components.end()) {
        auto mp = std::unordered_map<EntityId, Component *>();
        mp.emplace(entityId, component);
        m_Components[id] = mp;
    } else {
        findIt->second[entityId] = component;
    }
    entity->second->AddComponent<T>(component);

    for (auto &system: m_Systems) {
        system.second->OnEntityCreated(entity->second);
    }
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
        system = new(pSystemMem)T(std::forward<Args>(args)...);
        ((T *) pSystemMem)->m_EntityManager = this;
        this->m_Systems[id] = system;
        log_info("System \'%s\' (%d bytes) created.", typeid(T).name(), sizeof(T));
    } else {
        log_fatal("Unable to create system \'%s\' (%d bytes).", typeid(T).name(), sizeof(T));
    }
    return system;

}

template<class T>
EntityManager::ComponentContainer<T> *EntityManager::GetComponentContainer() {
    auto CID = GetComponentTypeID<T>();
    auto it = this->m_ComponentContainerRegistry.find(CID);
    ComponentContainer<T> *cc = nullptr;

    if (it == this->m_ComponentContainerRegistry.end()) {
        cc = new ComponentContainer<T>();
        this->m_ComponentContainerRegistry[CID] = cc;
    } else
        cc = static_cast<ComponentContainer<T> *>(it->second);
    assert(cc != nullptr && "Failed to create ComponentContainer<T>!");
    return cc;
}
