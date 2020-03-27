#include "fury/ecs/entity_manager.h"

EntityManager::EntityManager() {
    this->m_SystemAllocator = new SystemAllocator(
            ECS_SYSTEM_MEMORY_BUFFER_SIZE,
            Allocate(ECS_SYSTEM_MEMORY_BUFFER_SIZE, "SystemManager")
    );
}

EntityManager::~EntityManager() {

    log_info("Releasing remaining entities of type '%s' ...", m_EntityContainer->GetEntityContainerTypeName());
    delete m_EntityContainer;
    m_EntityContainer = nullptr;

    for (auto cc : this->m_ComponentContainerRegistry) {
        log_info("Releasing remaining entities of type '%s' ...", cc.second->GetComponentContainerTypeName());
        delete cc.second;
        cc.second = nullptr;
    }

    m_Systems.clear();
    Free((void *) this->m_SystemAllocator->GetMemoryAddress0());
    delete this->m_SystemAllocator;
    this->m_SystemAllocator = nullptr;
    log_info("Release EntityManager!");
    m_MemoryManager->CheckMemoryLeaks();
}

void EntityManager::CreateSystems() {
    for (auto &system: m_Systems) {
        system.second->Create();
    }
}

void EntityManager::Update() {
    for (auto &system: m_Systems) {
        if (system.second->m_ShouldUpdate) {
            system.second->Update();
        }
    }
}

EntityId EntityManager::CreateEntity() {
    void *pObjectMemory = GetEntityContainer()->CreateObject();
    auto a = new(pObjectMemory)Entity(m_EntityCnt);
    m_Entities.emplace(a->GetId(), a);
    m_EntityCnt++;
    return a->GetId();
}

void EntityManager::DestroyEntity(EntityId entityId) {
    auto entity = m_Entities.find(entityId);
    assert(entity != m_Entities.end());
    GetEntityContainer()->DestroyEntity(entity->second);
    m_Entities.erase(entityId);
    RemoveAllComponent(entityId);
    for (auto &system: m_Systems) {
        system.second->OnEntityDestroyed(entityId);
    }
}

EntityManager::EntityContainer<Entity> *EntityManager::GetEntityContainer() {
    EntityContainer<Entity> *ec = nullptr;

    if (m_EntityContainer == nullptr) {
        ec = new EntityContainer<Entity>();
        m_EntityContainer = ec;
    } else {

        ec = m_EntityContainer;
    }
    return ec;
}

void EntityManager::RemoveAllComponent(EntityId entityId) const {
    for (auto &cmd: m_Components) {
        auto components = cmd.second;
        auto it = components.find(entityId);
        if (it != components.end()) {
            auto container = this->m_ComponentContainerRegistry.find(cmd.first);
            if (container != this->m_ComponentContainerRegistry.end()) {
                container->second->DestroyComponent(it->second);
            }
        }
    }
}
