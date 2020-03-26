#pragma once

#include "component.h"
#include "fury/memory/memory_chunk_allocator.h"

template<class T>
class ComponentContainer : public MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE> {
public:
    ComponentContainer() : MemoryChunkAllocator("ComponentManager") {}

    ~ComponentContainer() {}

    ComponentContainer(const ComponentContainer &) = delete;

    ComponentContainer &operator=(const ComponentContainer &) = delete;

    ComponentContainer(ComponentContainer &&) = delete;

    ComponentContainer &operator=(ComponentContainer &&) = delete;

    const char *GetComponentContainerTypeName() const {
        static const char *COMPONENT_TYPE_NAME{typeid(T).name()};
        return COMPONENT_TYPE_NAME;
    }

    void DestroyComponent(Component *object) {
        object->Destroy();
        this->DestroyObject(object);
    }
};
