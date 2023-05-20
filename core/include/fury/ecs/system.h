#pragma once

#include "base_system.h"
#include "ecs_types.h"
#include "entity_manager.h"
#include "entity.h"
#include <map>

template<class ...Comps>
class System : public BaseSystem {
    friend class EntityManager;
    std::map<EntityId, size_t> m_EntityIdToIndexMap;
protected:
    EntityManager *m_EntityManager;
    using CompTuple = std::tuple<std::add_pointer_t<Comps>...>;
    std::vector<CompTuple> m_Components;
public:
    void OnEntityCreated(Entity *entity) override;

    void OnEntityDestroyed(EntityId entityId) override;

private:
    template<size_t Index, class CompType, class... CompArgs>
    bool CheckComponents(ComponentId componentId, Component *component, CompTuple &tpl);

    template<size_t Index>
    bool CheckComponents(ComponentId componentId, Component *component, CompTuple &tpl);
};

template<class... Comps>
void System<Comps...>::OnEntityCreated(Entity *entity) {
    CompTuple compTuple;
    size_t matchingSize = 0;
    for (auto &p: entity->GetComponents()) {
        if (CheckComponents<0, Comps...>(p.first, p.second, compTuple)) {
            ++matchingSize;
            if (matchingSize == sizeof...(Comps)) {
                m_Components.emplace_back(std::move(compTuple));
                m_EntityIdToIndexMap.emplace(entity->GetId(), m_Components.size() - 1);
                m_ShouldUpdate = m_Components.size() > 0;
                break;
            }
        }
    }
}

template<class... Comps>
void System<Comps...>::OnEntityDestroyed(EntityId entityId) {
    const auto findIt = m_EntityIdToIndexMap.find(entityId);
    if (findIt != m_EntityIdToIndexMap.end()) {
        m_Components[findIt->second] = std::move(m_Components.back());
        m_Components.pop_back();
        const auto *pMovedComp = std::get<0>(m_Components[findIt->second]);
        auto movedTupleIt = m_EntityIdToIndexMap.find(pMovedComp->GetEntityId());
        assert(movedTupleIt != m_EntityIdToIndexMap.end());
        movedTupleIt->second = findIt->second;
        m_ShouldUpdate = m_Components.size() > 0;
    }

}

template<class... Comps>
template<size_t Index, class CompType, class... CompArgs>
bool System<Comps...>::CheckComponents(ComponentId componentId, Component *component, System::CompTuple &tpl) {
    if (GetComponentTypeID<CompType>() == componentId) {
        std::get<Index>(tpl) = static_cast<CompType *>(component);
        return true;
    } else {
        return CheckComponents<Index + 1, CompArgs...>(componentId, component, tpl);
    }
}

template<class... Comps>
template<size_t Index>
bool System<Comps...>::CheckComponents(ComponentId componentId, Component *component, System::CompTuple &tpl) {
    return false;
}