#include "fury/ecs/entity_manager.h"

EntityManager::EntityManager() {

}

EntityManager::~EntityManager() {

}

void EntityManager::CreateSystems() {
    for (auto &system: m_Systems) {
        system->Create();

    }
}

void EntityManager::Update() {
    for (auto &system: m_Systems) {
        if(system->m_ShouldUpdate) {
            system->Update();
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
        system->OnEntityDestroyed(entityId);
    }
}


void EntityManager::AddEntity(EntityId entityId) {
    auto entity = m_Entities.find(entityId);
    assert(entity != m_Entities.end());
    for (auto &system: m_Systems) {
        system->OnEntityCreated(entity->second);
    }
}

