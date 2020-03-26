#pragma once

#include "ecs_types.h"
#include "ecs_types.h"
#include <type_traits>
#include <limits>

struct Component {
    static constexpr PoolId INVALID_POOL_ID = std::numeric_limits<PoolId>::max();
    EntityId m_EntityId;
    PoolId m_PoolId;
public:
    Component() : m_PoolId(INVALID_POOL_ID), m_EntityId(INVALID_ENTITY_ID) {};

    virtual ~Component() = default;

    virtual void Create() {};

    void SetEntityId(EntityId id) {
        m_EntityId = id;
    }

    const EntityId &GetEntityId() const {
        return m_EntityId;
    }

    void SetPoolId(PoolId id) {
        m_PoolId = id;
    }

    const PoolId &GetPoolId() const {
        return m_PoolId;
    }
};


namespace Internal {
    inline ComponentId GetUniqueComponentID() noexcept {
        static ComponentId lastID{0u};
        return lastID++;
    }
}

template<class T>
inline ComponentId GetComponentTypeID() noexcept {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    static ComponentId typeID{Internal::GetUniqueComponentID()};
    return typeID;
}
