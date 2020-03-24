//
// Created by aryan on 3/20/20.
// borrowed from
// https://github.com/tobias-stein/EntityComponentSystem
//

#ifndef GAME_POOL_ALLOCATOR_H
#define GAME_POOL_ALLOCATOR_H

#include "allocator.h"

class PoolAllocator : public Allocator {
private:

    const size OBJECT_SIZE;
    const u8 OBJECT_ALIGNMENT;

    void **freeList;

public:

    PoolAllocator(size memSize, const void *mem, size objectSize, u8 objectAlignment);

    ~PoolAllocator() override;

    void *allocate(size size, u8 alignment) override;

    void free(void *p) override;

    void clear() override;

};

#endif