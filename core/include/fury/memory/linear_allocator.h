#pragma once

#include "allocator.h"

class LinearAllocator : public Allocator {
public:

    LinearAllocator(size_t memSize, const void *mem);

    ~LinearAllocator() override;

    void *allocate(size_t size, u8 alignment) override;

    void free(void *p) override;

    void clear() override;

};
