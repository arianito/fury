//
// Created by Aryan on 10/30/18.
//

#ifndef FURY_ECS_ENTITY_COMPONENT_H
#define FURY_ECS_ENTITY_COMPONENT_H

#include "Fury/Primitives/System.h"

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <bitset>

class Entity;

namespace Internal{

    inline Segment getUniqueComponentId() noexcept {
        static Segment lastId{0u};
        return lastId++;
    }

    template<typename T>
    inline Segment getComponentId() noexcept {
        static Segment typeId{getComponentId()};
        return typeId;
    }
}



struct Component {
    Entity *entity{NULL};

    virtual void Update() {};

    virtual void Draw() {};

    virtual ~Component() {};
};


class Entity {
private:
    Segment m_id;
    bool m_alive{true};
    Vector<Unique<Component>> components;

public:
    void Update() {
        for (auto &c : components) {
            c->Update();
        }
    };

    void Draw() {
        for (auto &c : components) {
            c->Draw();
        }
    };


    Boolean &isAlive() const {
        return m_alive;
    }

    void Destroy() {
        m_alive = false;
    };


    virtual const EntityTypeId GetStaticEntityTypeID() const = 0;

    inline const EntityId GetEntityID() const { return this->id; }
};

#endif //FURY_ECS_ENTITY_COMPONENT_H
