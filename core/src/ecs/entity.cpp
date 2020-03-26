#include "fury/ecs/entity.h"

EntityId Entity::GetId() const { return m_Id; }


const Entity::ComponentMap &Entity::GetComponents() const {
    return m_Components;
}
