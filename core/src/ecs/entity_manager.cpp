#include "fury/ecs/entity_manager.h"

EntityManager::EntityManager() {
    this->m_SystemAllocator = new SystemAllocator(ECS_SYSTEM_MEMORY_BUFFER_SIZE, Allocate(ECS_SYSTEM_MEMORY_BUFFER_SIZE, "SystemManager"));
}

EntityManager::~EntityManager() {
    for (auto &system: m_Systems) {
        system.second->Destroy();
        system.second = nullptr;
    }
    m_Systems.clear();
    Free((void*)this->m_SystemAllocator->GetMemoryAddress0());
    delete this->m_SystemAllocator;
    this->m_SystemAllocator = nullptr;
}

void EntityManager::CreateSystems() {
    for (auto &system: m_Systems) {
        system.second->Create();
    }
}

void EntityManager::Update() {
    for (auto &system: m_Systems) {
        if(system.second->m_ShouldUpdate) {
            system.second->Update();
        }
    }
}

EntityId EntityManager::CreateEntity() {
    auto a = new Entity(m_EntityCnt);
    m_Entities.emplace(a->GetId(), a);
    m_EntityCnt++;
    return a->GetId();
}

void EntityManager::DestroyEntity(EntityId entityId) {
    for (auto &system: m_Systems) {
        system.second->OnEntityDestroyed(entityId);
    }
}


void EntityManager::AddEntity(EntityId entityId) {
    auto entity = m_Entities.find(entityId);
    assert(entity != m_Entities.end());
    for (auto &system: m_Systems) {
        system.second->OnEntityCreated(entity->second);
    }
}

