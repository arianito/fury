//
// Created by aryan on 3/20/20.
//

#ifndef GAME_STACK_ALLOCATOR_H
#define GAME_STACK_ALLOCATOR_H

#include "allocator.h"

class StackAllocator : public Allocator {
private:

    struct AllocMetaInfo {
        u8 adjustment;
    };

public:

    StackAllocator(size memSize, const void *mem);

    ~StackAllocator() override;

    void *allocate(size size, u8 alignment) override;

    void free(void *p) override;

    void clear() override;

};

#endif