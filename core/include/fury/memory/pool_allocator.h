#pragma once

#include "allocator.h"

class PoolAllocator : public Allocator {
private:

    const size_t OBJECT_SIZE;
    const u8 OBJECT_ALIGNMENT;

    void **freeList;

public:

    PoolAllocator(size_t memSize, const void *mem, size_t objectSize, u8 objectAlignment);

    ~PoolAllocator() override;

    void *allocate(size_t size, u8 alignment) override;

    void free(void *p) override;

    void clear() override;

};