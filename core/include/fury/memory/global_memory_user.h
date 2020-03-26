#pragma once

#include "memory_manager.h"

class GlobalMemoryUser {
private:
    MemoryManager *ECS_MEMORY_MANAGER = nullptr;

public:
    virtual inline const void *allocate(size_t memSize, const char *user) = 0;

    virtual inline void free(void *pMem) = 0;
};