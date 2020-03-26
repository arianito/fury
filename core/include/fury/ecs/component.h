#pragma once

#include "ecs_types.h"
#include "ecs_types.h"
#include <type_traits>
#include <limits>

#define COMPONENT_LUT_GROW					1024

#define COMPONENT_T_CHUNK_SIZE				512

struct Component {
    EntityId m_EntityId;
public:
    Component() : m_EntityId(INVALID_ENTITY_ID) {};

    virtual ~Component() = default;

    virtual void Create() {};
    virtual void Destroy() {};

    void SetEntityId(EntityId id) {
        m_EntityId = id;
    }

    const EntityId &GetEntityId() const {
        return m_EntityId;
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
