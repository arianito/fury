#pragma once

#include "allocator.h"

class StackAllocator : public Allocator {
private:

    struct AllocMetaInfo {
        u8 adjustment;
    };

public:

    StackAllocator(size_t memSize, const void *mem);

    ~StackAllocator() override;

    void *Allocate(size_t size, u8 alignment) override;

    void Free(void *p) override;

    void Clear() override;

};