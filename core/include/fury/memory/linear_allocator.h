#pragma once

#include "allocator.h"

class LinearAllocator : public Allocator {
public:

    LinearAllocator(size_t memSize, const void *mem);

    ~LinearAllocator() override;

    void *Allocate(size_t size, u8 alignment) override;

    void Free(void *p) override;

    void Clear() override;

};
