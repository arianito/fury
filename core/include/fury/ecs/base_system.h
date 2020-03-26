#pragma once

#include "entity.h"

class BaseSystem {
public:
    bool m_ShouldUpdate = false;
    virtual ~BaseSystem() = default;

    virtual void Create() {};

    virtual void Update() {};

    virtual void OnEntityCreated(Entity *entity) = 0;

    virtual void OnEntityDestroyed(EntityId entityId) = 0;
};

namespace Internal {
    inline SystemId GetUniqueSystemID() noexcept {
        static SystemId lastID{0u};
        return lastID++;
    }
}

template<class T>
inline SystemId GetSystemTypeID() noexcept {
    static_assert(std::is_base_of<BaseSystem, T>::value, "T must inherit from System");
    static SystemId typeID{Internal::GetUniqueComponentID()};
    return typeID;
}

