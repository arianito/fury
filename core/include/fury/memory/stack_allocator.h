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

    void *allocate(size_t size, u8 alignment) override;

    void free(void *p) override;

    void clear() override;

};