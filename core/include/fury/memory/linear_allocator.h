//
// Created by aryan on 3/20/20.
// borrowed from
// https://github.com/tobias-stein/EntityComponentSystem
//

#ifndef GAME_LINEAR_ALLOCATOR_H
#define GAME_LINEAR_ALLOCATOR_H

#include "allocator.h"

class LinearAllocator : public Allocator {
public:

    LinearAllocator(size memSize, const void *mem);

    ~LinearAllocator() override;

    void *allocate(size size, u8 alignment) override;

    void free(void *p) override;

    void clear() override;

};

#endif